
#include <ArduinoDrivers/button.hpp>
#include <ArduinoDrivers/buttonTimed.hpp>
#include <ArduinoDrivers/buttonTimedMultiple.hpp>
#include <ArduinoDrivers/simplePinBit.hpp>

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string.h>

static constexpr SimplePin::State downState = SimplePin::State::Zero;
static constexpr SimplePin::State upState = SimplePin::State::One;
static constexpr ButtonTimedProperties::Duration_t durationShort = 2; // 50ms per cycle
static constexpr ButtonTimedProperties::Duration_t durationLong = 8;
static constexpr ButtonTimedProperties::Duration_t durationCombineMax = 6;

static uint8_t buttonsMemory[1];

typedef SimplePinBit<0, buttonsMemory> pin0;
typedef SimplePinBit<1, buttonsMemory> pin1;

typedef Button<pin0, downState> button0;
typedef Button<pin1, downState> button1;

typedef ButtonTimed<button0, durationShort, durationLong> buttonTimed0;
typedef ButtonTimed<button1, durationShort, durationLong> buttonTimed1;

typedef ButtonTimedMultiple<button0, durationShort, durationLong, durationCombineMax> buttonTimedMultiple0;
typedef ButtonTimedMultiple<button0, durationShort, durationLong, durationCombineMax> buttonTimedMultiple1;

int main(int argc, char* argv[])
{
    static_assert(0 < sizeof(buttonsMemory));
    memset(buttonsMemory, 0, sizeof(buttonsMemory) / sizeof(buttonsMemory[0]));

    // button
    pin0::set(upState);
    assert(button0::isUp());
    assert(!button0::isDown());

    pin0::set(downState);
    assert(!button0::isUp());
    assert(button0::isDown());

    // buttonTimed
    pin0::set(upState);
    buttonTimed0::update();

    pin0::set(downState);
    assert(!buttonTimed0::isDownShort());
    for (size_t index = 0; index < durationShort; ++index)
    {
        buttonTimed0::update();
    }
    assert(!buttonTimed0::isUp());
    assert(buttonTimed0::isDown());
    assert(buttonTimed0::isDownShort());
    assert(!buttonTimed0::isDownLong());
    for (size_t index = 0; index < (durationLong - durationShort); ++index)
    {
        buttonTimed0::update();
    }
    assert(!buttonTimed0::isUp());
    assert(buttonTimed0::isDown());
    assert(!buttonTimed0::isDownShort());
    assert(buttonTimed0::isDownLong());

    // buttonTimedMultiple
    buttonTimedMultiple0::clearHistory();
    pin0::set(downState);
    assert(!buttonTimedMultiple0::isDownShort());
    for (size_t index = 0; index < durationShort; ++index)
    {
        buttonTimedMultiple0::update();
    }
    assert(!buttonTimedMultiple0::isUp());
    assert(buttonTimedMultiple0::isDown());
    assert(buttonTimedMultiple0::isDownShort());
    assert(!buttonTimedMultiple0::isDownLong());
    for (size_t index = 0; index < (durationLong - durationShort); ++index)
    {
        buttonTimedMultiple0::update();
    }
    assert(!buttonTimedMultiple0::isUp());
    assert(buttonTimedMultiple0::isDown());
    assert(!buttonTimedMultiple0::isDownShort());
    assert(buttonTimedMultiple0::isDownLong());


    std::cout << "The End." << std::endl;
}
