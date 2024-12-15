
#include <ArduinoDrivers/button.hpp>
#include <ArduinoDrivers/simplePinBit.hpp>

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string.h>

static constexpr SimplePin::State downState = SimplePin::State::Zero;
static constexpr SimplePin::State upState = SimplePin::State::One;

static uint8_t buttonsMemory[1];

typedef SimplePinBit<0, buttonsMemory> pin0;
typedef SimplePinBit<1, buttonsMemory> pin1;

typedef Button<pin0, downState> button0;
typedef Button<pin1, downState> button1;


int main(int argc, char* argv[])
{
    static_assert(0 < sizeof(buttonsMemory));
    memset(buttonsMemory, 0, sizeof(buttonsMemory) / sizeof(buttonsMemory[0]));

    pin0::set(upState);
    assert(button0::isUp());
    assert(!button0::isDown());

    pin0::set(downState);
    assert(!button0::isUp());
    assert(button0::isDown());


    std::cout << "Hello, World!" << std::endl;
}
