#include "dollHouseButtons.hpp"
#include "dollHouseStatemachine.hpp"

#if defined(_AVR_IOTNX4_H_)
#include "ArduinoDrivers/attinyX4.hpp"
#elif defined(_AVR_IOM328P_H_)
#include "ArduinoDrivers/arduinoUno.hpp"
#else
#error "Unsupported board selected."
#endif
#include "ArduinoDrivers/dummytypes.hpp"

#include "ArduinoDrivers/parallelinshiftregister74hc165.hpp"

#include "colors/colorRgbw.cpp"
#include "colors/sevenSegmentRgb.hpp"
#include "colors/sevenSegmentRgb.cpp"

#include "helpers/crc16.hpp"
#include "helpers/tmpLoop.hpp"

#ifdef _AVR_IOTNX4_H_
// Attiny84 with port A-pin requires a specially modified NeoPixel-library.
#include <Adafruit_NeoPixel_PortA.h>
#else
#include <Adafruit_NeoPixel.h>
#endif

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include <stdint.h>
#include <string.h>


// https://stackoverflow.com/questions/6938219/how-to-check-whether-all-bytes-in-a-memory-block-are-zero
static int memvcmp(uint8_t const * memory, uint8_t const value, size_t byteLength)
{
    if (0 == byteLength)
    {
        return 0;
    }
    else
    {
        int const firstByteComparison = memcmp(memory, &value, 1);
        return (0 == firstByteComparison) ? memcmp(memory, memory + 1, byteLength - 1) : firstByteComparison;
    }
}


uint8_t constexpr shiftRegisterBitsCount = 8;
static_assert(shiftRegisterBitsCount >= DollHouse::numberOfButtons);

typedef ParallelInShiftRegister74HC165<shiftRegisterBitsCount,
#if defined(_AVR_IOTNX4_H_)
                                       ATtinyX4::pinB0,
                                       ATtinyX4::pinB1,
                                       DummyAvrPin1,
                                       ATtinyX4::pinB2,
#elif defined(_AVR_IOM328P_H_)
                                       ArduinoUno::A3,
                                       ArduinoUno::A2,
                                       DummyAvrPin1,
                                       ArduinoUno::A4,
#else
    // Not supported yet.
#endif
                                       DummyAvrPin1,
                                       DummyAvrPin1> buttonsInShiftRegister;

int constexpr pinLedsStrip =
#if defined(_AVR_IOTNX4_H_)
        ATtinyX4::pinA0::pinNumber
#elif defined(_AVR_IOM328P_H_)
        ArduinoUno::D6::pinNumber
#else
    // Not supported yet.
#endif
    ;

uint16_t constexpr ledsCount = 9;
static Adafruit_NeoPixel ledsStrip(ledsCount, pinLedsStrip, NEO_GRBW + NEO_KHZ800);


// template<uint8_t Index>
// struct WrapperLogButton
// {
//     static void impl()
//     {
//         if (DollHouse::Buttons<Index>::isDown())
//         {
//             Serial.print("0");
//         }
//         else
//         {
//             Serial.print("1");
//         }
//     }
// };


namespace Eeprom
{

typedef size_t Address;

namespace Addresses
{

static Address constexpr backupValues = 0;

} // namespace Addresses


void writeWithCrc(void const * const data, size_t const byteCount, Address const eepromAddress)
{
    Crc16Ibm3740 crc;
    crc.process(static_cast<uint8_t const *>(data), byteCount);
    uint16_t const crcValue = crc.get();

    eeprom_write_block(data, (void *)(eepromAddress), byteCount);
    eeprom_write_block(&crcValue, (void *)(eepromAddress + byteCount), 2);
}


bool readWithCrc(void * const data, size_t const byteCount, Address const eepromAddress)
{
    uint16_t crcValue = 0xffff;

    eeprom_read_block(data, (void const *)(eepromAddress), byteCount);
    eeprom_read_block(&crcValue, (void const *)(eepromAddress + byteCount), 2);

    Crc16Ibm3740 crc;
    crc.process(static_cast<uint8_t const *>(data), byteCount);

    return (crc.get() == crcValue);
}

} // namespace Eeprom


void setup()
{
    // initialize
    ledsStrip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    ledsStrip.show();            // Turn OFF all pixels ASAP
    // ledsStrip.setBrightness(Adafruit_NeoPixel::gamma8(255));

    // Serial.begin(9600);
    // Serial.println("Doll house v0.9");

    buttonsInShiftRegister::initialize();
    buttonsInShiftRegister::enableClock();

    // load settings from EEPROM
    // Load from Eeprom.
    bool const readBack = Eeprom::readWithCrc(DollHouse::settingsColors, sizeof(DollHouse::settingsColors), Eeprom::Addresses::backupValues);
    if (!readBack)
    {
        // Default to full white explicitely.
        for (size_t index = 0; index < DollHouse::numberOfButtons; ++index)
        {
            DollHouse::settingsColors[index] = Colors::ColorCustomFixed(0, 255);
        }
    }

    // Show power-up on Status LED.
    DollHouse::displayColors[0] = Colors::ColorCustomFixed(65536/7*4+500, 12);

    // loop
    while (true)
    {
        // Latch bits in shift-register for read-out from the buttons.
        buttonsInShiftRegister::loadParallelToShiftregister();
        // Actually copy the latched shift-register values to data.
        buttonsInShiftRegister::shiftOutBits(DollHouse::buttonsMemory);

        for (DollHouse::CustomButtomTimedMultiple & button : DollHouse::buttonsTimedMultiple)
        {
            button.update();
        }

        // Helpers::TMP::Loop<DollHouse::numberOfButtons, WrapperLogButton>::impl();
        // Serial.println(".");

        for (size_t index = 0; index < DollHouse::numberOfButtons; ++index)
        {
            DollHouse::statemachines[index].process(DollHouse::dataTypes[index]);
            // // For the following to work, you will have to remove the "private:" status
            // // of currentState_ in Statemachine. Only do so temporarily!
            // if (&DollHouse::stateBrightness == statemachines[index].currentState_)
            // {
            //     Serial.print("Brightness.");
            // }
            // else if (&DollHouse::stateHue == statemachines[index].currentState_)
            // {
            //     Serial.print("Hue.");
            // }
            // else if (&DollHouse::stateOn == statemachines[index].currentState_)
            // {
            //     Serial.print("On.");
            // }
            // else if (&DollHouse::stateOff == statemachines[index].currentState_)
            // {
            //     Serial.print("Off.");
            // }
        }
        // Serial.println("");

        if (DollHouse::updateDisplay)
        {
            // convert Colors::ColorCustomFixed to RGB
            for (size_t index = 0; index < ledsStrip.numPixels(); ++index)
            {
                Colors::ColorRgbw const colorRgb = Colors::SevenSegmentRgb::toRgb(DollHouse::displayColors[index]);
                ledsStrip.setPixelColor(index, colorRgb.red, colorRgb.green, colorRgb.blue);
            }
            // show NEO-pixels
            ledsStrip.show();
            DollHouse::updateDisplay = false;
        }

        if (DollHouse::saveSettings)
        {
            // // Save to Eeprom.
            static_assert(E2END >= (Eeprom::Addresses::backupValues + sizeof(DollHouse::settingsColors) + 2 /* CRC */ - 1 /* index */));
            Eeprom::writeWithCrc(DollHouse::settingsColors, sizeof(DollHouse::settingsColors), Eeprom::Addresses::backupValues);
            DollHouse::saveSettings = false;
        }

        delay(50); // idle for 50ms
    }

    // Notify error - one should never leave the while-loop above.
    ledsStrip.setPixelColor(0, /*red*/ 255, /*green*/ 0, /*blue*/ 0);
    ledsStrip.show();
}

void loop()
{
    // intentionally empty
}
