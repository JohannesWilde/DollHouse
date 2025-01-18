
#include <ArduinoDrivers/button.hpp>
#include <ArduinoDrivers/buttonDynamic.hpp>
#include <ArduinoDrivers/buttonTimedProperties.hpp>
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

static uint8_t buttonsMemory[1] = {0, };

typedef SimplePinBit<0, buttonsMemory> pin0;
typedef SimplePinBit<1, buttonsMemory> pin1;

typedef Button<pin0, downState> button0;
typedef Button<pin1, downState> button1;


static void buttonInitialize(void const * const instance);
static void buttonDeinitialize(void const * const instance);
static bool buttonIsDown(void const * const instance);

typedef ButtonDynamic<&buttonInitialize, &buttonIsDown, &buttonDeinitialize> CustomButton;
typedef ButtonTimedMultiple<CustomButton, durationShort, durationLong, durationCombineMax, 5> CustomButtomTimedMultiple;

static CustomButtomTimedMultiple buttonTimedMultiple0;
static CustomButtomTimedMultiple buttonTimedMultiple1;



static void buttonInitialize(void const * const instance)
{
    if (static_cast<CustomButtomTimedMultiple const *>(instance) == &buttonTimedMultiple0)
    {
        button0::initialize();
    }
    else if (static_cast<CustomButtomTimedMultiple const *>(instance) == &buttonTimedMultiple1)
    {
        button1::initialize();
    }
    else
    {
        assert(false);
    }
}

static void buttonDeinitialize(void const * const instance)
{
    if (static_cast<CustomButtomTimedMultiple const *>(instance) == &buttonTimedMultiple0)
    {
        button0::deinitialize();
    }
    else if (static_cast<CustomButtomTimedMultiple const *>(instance) == &buttonTimedMultiple1)
    {
        button1::deinitialize();
    }
    else
    {
        assert(false);
    }
}

static bool buttonIsDown(void const * const instance)
{
    if (static_cast<CustomButtomTimedMultiple const *>(instance) == &buttonTimedMultiple0)
    {
        return button0::isDown();
    }
    else if (static_cast<CustomButtomTimedMultiple const *>(instance) == &buttonTimedMultiple1)
    {
        return button1::isDown();
    }
    else
    {
        assert(false);
    }
}


int main(int argc, char* argv[])
{
    // initialize
    static_assert(0 < sizeof(buttonsMemory));
    memset(buttonsMemory, 0, sizeof(buttonsMemory) / sizeof(buttonsMemory[0]));

    // button
    pin0::set(upState);
    assert(button0::isUp());
    assert(!button0::isDown());

    pin0::set(downState);
    assert(!button0::isUp());
    assert(button0::isDown());


    // buttonTimedMultiple
    buttonTimedMultiple0.clearHistory();
    pin0::set(downState);
    assert(!buttonTimedMultiple0.isDownShort());
    for (size_t index = 0; index < durationShort; ++index)
    {
        buttonTimedMultiple0.update();
    }
    assert(!buttonTimedMultiple0.isUp());
    assert(buttonTimedMultiple0.isDown());
    assert(buttonTimedMultiple0.isDownShort());
    assert(!buttonTimedMultiple0.isDownLong());
    for (size_t index = 0; index < (durationLong - durationShort); ++index)
    {
        buttonTimedMultiple0.update();
    }
    assert(!buttonTimedMultiple0.isUp());
    assert(buttonTimedMultiple0.isDown());
    assert(!buttonTimedMultiple0.isDownShort());
    assert(buttonTimedMultiple0.isDownLong());

    // double click
    buttonTimedMultiple0.clearHistory();
    assert(!buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(!buttonTimedMultiple0.isSingleDownShortFinished());
    pin0::set(upState);
    for (size_t index = 0; index < durationLong; ++index)
    {
        buttonTimedMultiple0.update();
    }
    assert(!buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(!buttonTimedMultiple0.isSingleDownShortFinished());
    pin0::set(downState);
    for (size_t index = 0; index < durationShort; ++index)
    {
        buttonTimedMultiple0.update();
    }
    assert(!buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(!buttonTimedMultiple0.isSingleDownShortFinished());
    pin0::set(upState);
    for (size_t index = 0; index < durationCombineMax; ++index)
    {
        buttonTimedMultiple0.update();
    }
    assert(!buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(!buttonTimedMultiple0.isSingleDownShortFinished());
    pin0::set(downState);
    for (size_t index = 0; index < durationShort; ++index)
    {
        buttonTimedMultiple0.update();
    }
    assert(!buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(!buttonTimedMultiple0.isSingleDownShortFinished());
    pin0::set(upState);
    for (size_t index = 0; index < durationCombineMax; ++index)
    {
        buttonTimedMultiple0.update();
    }
    assert(!buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(!buttonTimedMultiple0.isSingleDownShortFinished());
    // double finished
    buttonTimedMultiple0.update();
    assert(buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(!buttonTimedMultiple0.isSingleDownShortFinished());
    // forget with time
    buttonTimedMultiple0.update();
    assert(!buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(!buttonTimedMultiple0.isSingleDownShortFinished());

    // single click
    buttonTimedMultiple0.clearHistory();
    assert(!buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(!buttonTimedMultiple0.isSingleDownShortFinished());
    pin0::set(upState);
    for (size_t index = 0; index < durationLong; ++index)
    {
        buttonTimedMultiple0.update();
    }
    assert(!buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(!buttonTimedMultiple0.isSingleDownShortFinished());
    pin0::set(downState);
    for (size_t index = 0; index < durationShort; ++index)
    {
        buttonTimedMultiple0.update();
    }
    assert(!buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(!buttonTimedMultiple0.isSingleDownShortFinished());
    pin0::set(upState);
    for (size_t index = 0; index < durationCombineMax; ++index)
    {
        buttonTimedMultiple0.update();
    }
    assert(!buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(!buttonTimedMultiple0.isSingleDownShortFinished());
    // finished single
    buttonTimedMultiple0.update();
    assert(!buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(buttonTimedMultiple0.isSingleDownShortFinished());
    // forget with time
    buttonTimedMultiple0.update();
    assert(!buttonTimedMultiple0.isDoubleDownShortFinished());
    assert(!buttonTimedMultiple0.isSingleDownShortFinished());


    // check iterability
    bool anyButtonIsDown = false;
    for (CustomButtomTimedMultiple const & buttonTimedMultiple : {buttonTimedMultiple0, buttonTimedMultiple1})
    {
        anyButtonIsDown |= buttonTimedMultiple.isDown();
    }


    std::cout << "The End." << std::endl;
}
