#include "dollHouseButtons.hpp"
#include "dollHouseStatemachine.hpp"

#include "ArduinoDrivers/attinyX4.hpp"
#include "ArduinoDrivers/dummytypes.hpp"

#include "ArduinoDrivers/parallelinshiftregister74hc165.hpp"

#include "colors/colorRgbw.cpp"
#include "colors/sevenSegmentRgb.hpp"
#include "colors/sevenSegmentRgb.cpp"

#include "helpers/crc16.hpp"
#include "helpers/tmpLoop.hpp"

#include <Adafruit_NeoPixel_PortA.h>

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
                                       ATtinyX4::pinB0,
                                       ATtinyX4::pinB1,
                                       DummyAvrPin1,
                                       ATtinyX4::pinB2,
                                       DummyAvrPin1,
                                       DummyAvrPin1> buttonsInShiftRegister;


int constexpr pinLedsStrip = ATtinyX4::pinA0::pinNumber;
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

    static_assert(0 < sizeof(DollHouse::buttonsMemory));
    memset(DollHouse::buttonsMemory, 0, sizeof(DollHouse::buttonsMemory) / sizeof(DollHouse::buttonsMemory[0]));
    Helpers::TMP::Loop<DollHouse::numberOfButtons, DollHouse::WrapperInitialize>::impl();

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

    // statemachine
     DollHouse::DataType dataTypes[DollHouse::numberOfButtons] = {
        {1, 0, },
        {2, 1, },
        {3, 2, },
        {4, 3, },
        {5, 4, },
        {6, 5, },
        {7, 6, },
        {8, 7, },
        };

    Helpers::Statemachine<DollHouse::DataType> statemachines[DollHouse::numberOfButtons] = {
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
    };

    // loop
    while (true)
    {
        // Latch bits in shift-register for read-out from the buttons.
        buttonsInShiftRegister::loadParallelToShiftregister();
        // Actually copy the latched shift-register values to data.
        buttonsInShiftRegister::shiftOutBits(DollHouse::buttonsMemory);

        Helpers::TMP::Loop<DollHouse::numberOfButtons, DollHouse::WrapperUpdate>::impl();

        // Helpers::TMP::Loop<DollHouse::numberOfButtons, WrapperLogButton>::impl();
        // Serial.println(".");

        for (size_t index = 0; index < DollHouse::numberOfButtons; ++index)
        {
            statemachines[index].process(dataTypes[index]);
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
}

void loop()
{
    // Notify error
    // initialize digital pin LED_BUILTIN as an output.
    // pinMode(LED_BUILTIN, OUTPUT);
}
