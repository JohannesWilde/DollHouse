
#include <ArduinoDrivers/button.hpp>
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
typedef Button<pin0, downState> button0;
typedef ButtonTimedMultiple<button0, durationShort, durationLong, durationCombineMax> buttonTimedMultiple0;

int main(int argc, char* argv[])
{
    // initialize
    static_assert(0 < sizeof(buttonsMemory));
    memset(buttonsMemory, 0, sizeof(buttonsMemory) / sizeof(buttonsMemory[0]));
    pin0::initialize();
    buttonTimedMultiple0::initialize();


    // statemachine
    assert(true);


    // deinitialize
    pin0::deinitialize();
    buttonTimedMultiple0::deinitialize();

    std::cout << "The End." << std::endl;
}
