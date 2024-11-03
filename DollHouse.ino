#include "ArduinoDrivers/ArduinoUno.hpp"
#include "ArduinoDrivers/dummytypes.hpp"

#include "ArduinoDrivers/parallelinshiftregister74hc165.hpp"

#include "ArduinoDrivers/button.hpp"
#include "ArduinoDrivers/buttonTimed.hpp"
#include "ArduinoDrivers/simplePinAvr.hpp"
#include "ArduinoDrivers/simplePinBit.hpp"

#include "helpers/crc16.hpp"
#include "helpers/tmpLoop.hpp"

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

uint8_t incrementUint8Capped(uint8_t const value)
{
    uint8_t newValue = value;
    if (newValue < UCHAR_MAX)
    {
        ++newValue;
    }
    return newValue;
}


uint8_t constexpr shiftRegisterBitsCount = 8;

typedef ParallelInShiftRegister74HC165<shiftRegisterBitsCount,
                                       ArduinoUno::A3,
                                       ArduinoUno::A2,
                                       DummyAvrPin1,
                                       ArduinoUno::A4,
                                       DummyAvrPin1,
                                       DummyAvrPin1> buttonsInShiftRegister;


uint8_t constexpr shortPressCount = 2;
uint8_t constexpr longPressCount = 8;

static uint8_t dataIn[1] = {0x00, };

template <uint8_t index>
class Buttons;

template <> class Buttons< 0> : public ButtonTimed<Button<SimplePinBitRead<0, dataIn, 0>, SimplePin::State::Zero>, shortPressCount, longPressCount> {/* intentionally empty */};
template <> class Buttons< 1> : public ButtonTimed<Button<SimplePinBitRead<1, dataIn, 0>, SimplePin::State::Zero>, shortPressCount, longPressCount> {/* intentionally empty */};
template <> class Buttons< 2> : public ButtonTimed<Button<SimplePinBitRead<2, dataIn, 0>, SimplePin::State::Zero>, shortPressCount, longPressCount> {/* intentionally empty */};
template <> class Buttons< 3> : public ButtonTimed<Button<SimplePinBitRead<3, dataIn, 0>, SimplePin::State::Zero>, shortPressCount, longPressCount> {/* intentionally empty */};
template <> class Buttons< 4> : public ButtonTimed<Button<SimplePinBitRead<4, dataIn, 0>, SimplePin::State::Zero>, shortPressCount, longPressCount> {/* intentionally empty */};
template <> class Buttons< 5> : public ButtonTimed<Button<SimplePinBitRead<5, dataIn, 0>, SimplePin::State::Zero>, shortPressCount, longPressCount> {/* intentionally empty */};
template <> class Buttons< 6> : public ButtonTimed<Button<SimplePinBitRead<6, dataIn, 0>, SimplePin::State::Zero>, shortPressCount, longPressCount> {/* intentionally empty */};
template <> class Buttons< 7> : public ButtonTimed<Button<SimplePinBitRead<7, dataIn, 0>, SimplePin::State::Zero>, shortPressCount, longPressCount> {/* intentionally empty */};


// Wrappers for loops.
template<uint8_t Index>
struct WrapperInitialize
{
    static void impl()
    {
        Buttons<Index>::initialize();
    }
};


template<uint8_t Index>
struct WrapperDeinitialize
{
    static void impl()
    {
        Buttons<Index>::deinitialize();
    }
};


template<uint8_t Index>
struct WrapperUpdate
{
    static void impl()
    {
        Buttons<Index>::update();
    }
};


template<uint8_t Index>
struct WrapperLogButton
{
    static void impl()
    {
        static bool wasDown = false;
        static bool wasDownLong = false;

        if (!wasDown && Buttons<Index>::isDown())
        {
            wasDown = true;
            Serial.print("Button ");
            Serial.print(Index);
            Serial.print(" down short");
            Serial.println(".");
        }
        else if (!wasDownLong && Buttons<Index>::isDownLong())
        {
            Serial.print("Button ");
            Serial.print(Index);
            Serial.print(" down long");
            Serial.println(".");
        }

        wasDown = Buttons<Index>::isDown();
        wasDownLong = Buttons<Index>::isDownLong();
    }
};



enum class Mode
{
    Default
};


template<uint8_t Index>
struct WrapperCountButtonsReleasedAfterShort
{
    static_assert(shiftRegisterBitsCount > Index);

    static void impl(uint8_t & count)
    {
        if (Buttons<Index>::releasedAfterShort())
        {
            ++count;
        }
    }
};

/* Never use this for actual HW buttons - but only for the update from a level input. */
template<uint8_t Index>
struct WrapperCountButtonsPressed_
{
    static_assert(shiftRegisterBitsCount > Index);

    static void impl(uint8_t & count)
    {
        if (Button<SimplePinBitRead<Index % 8, dataIn, Index / 8>, SimplePin::State::One>::isDown())
        {
            ++count;
        }
    }
};


struct BackupValues
{
    // uint8_t dataOut[4];
    Mode mode;

    BackupValues(Mode const mode)
        : mode(mode)
    {
        // memcpy(dataOut, dataOut_, sizeof(dataOut));
    }
};


namespace Eeprom
{

typedef size_t Address;

namespace Addresses
{

static Address constexpr backupValues = 0;

static_assert(E2END >= (backupValues + sizeof(BackupValues) + 2 /* CRC */ - 1 /* index */));

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
    Serial.begin(9600);
    Serial.println("Doll house v0.1");

    buttonsInShiftRegister::initialize();
    buttonsInShiftRegister::enableClock();

    Mode mode = Mode::Default;

    // // Save to Eeprom.
    // BackupValues backupValues(mode);
    // Eeprom::writeWithCrc(&backupValues, sizeof(BackupValues), Eeprom::Addresses::backupValues);

    // // Load from Eeprom.
    // BackupValues backupValues(mode);
    // bool const readBack = Eeprom::readWithCrc(&backupValues, sizeof(BackupValues), Eeprom::Addresses::backupValues);
    // if (!readBack)
    // {
    //     backupValues = BackupValues(mode);
    // }
    // else
    // {
    //     // Use default values explicitely.
    //     mode = backupValues.mode;
    // }

    while (true)
    {
        // Latch bits in shift-register for read-out from the buttons.
        buttonsInShiftRegister::loadParallelToShiftregister();
        // Actually copy the latched shift-register values to data.
        buttonsInShiftRegister::shiftOutBits(dataIn);

        Helpers::TMP::Loop<8, WrapperUpdate>::impl();

        switch (mode)
        {
        case Mode::Default:
        {
            Helpers::TMP::Loop<8, WrapperLogButton>::impl();

            // if (Buttons<0>::isDown())
            // {
            //     // light up LED red
            // }
            // else if (Buttons<0>::isDownLong())
            // {
            //     // light up LED green
            // }

            break;
        }
        }

        // displayDataOut();

        delay(50); // idle for 50ms
    }
}

void loop()
{
    // Notify error
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
  
    while (true)
    {
        digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
        delay(1000);                      // wait for a second
        digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
        delay(1000);                      // wait for a second
    }
}
