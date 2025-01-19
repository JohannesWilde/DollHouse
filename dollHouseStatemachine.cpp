#include "dollHouseStatemachine.hpp"

#include "dollHouseButtons.hpp"


namespace DollHouse
{

StateOff const stateOff;
StateOn const stateOn;
StateBrightness const stateBrightness;
StateHue const stateHue;


Colors::ColorCustomFixed settingsColors[ledsCount] = {};
bool saveSettings = false;
Colors::ColorCustomFixed displayColors[ledsCount] = {};
bool updateDisplay = false;

DataType dataTypes[numberOfButtons] = {};

constexpr size_t getLedIndex(DataType const * const data)
{
    return 1 + (data - dataTypes);
}

constexpr size_t getButtonIndex(DataType const * const data)
{
    return (data - dataTypes);
}

Helpers::Statemachine<DataType> statemachines[numberOfButtons] = {
    Helpers::Statemachine(stateOff),
    Helpers::Statemachine(stateOff),
    Helpers::Statemachine(stateOff),
    Helpers::Statemachine(stateOff),
    Helpers::Statemachine(stateOff),
    Helpers::Statemachine(stateOff),
    Helpers::Statemachine(stateOff),
    Helpers::Statemachine(stateOff),
};


static constexpr uint8_t brightnessStepSingle = 1;
static constexpr uint8_t brightnessStep = 2 * brightnessStepSingle;
static constexpr uint8_t brightnessMin = 2 * brightnessStepSingle;


void StateOff::init(DataType & data) const
{
    displayColors[getLedIndex(&data)] = Colors::ColorCustomFixed(0, 0);
    updateDisplay = true;
}

Helpers::AbstractState<DataType> const & StateOff::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;
    if (buttonsTimedMultiple[getButtonIndex(&data)].isSingleDownShortFinished())
    {
        nextState = &stateOn;
    }
    else
    {
        // intentionally empty
    }
    return *nextState;
}

void StateOff::deinit(DataType & data) const
{
    // intentionally empty
}


void StateOn::init(DataType & data) const
{
    displayColors[getLedIndex(&data)] = settingsColors[getLedIndex(&data)];
    updateDisplay = true;
}

Helpers::AbstractState<DataType> const & StateOn::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;
    if (buttonsTimedMultiple[getButtonIndex(&data)].isSingleDownShortFinished())
    {
        nextState = &stateOff;
    }
    else if (buttonsTimedMultiple[getButtonIndex(&data)].isDownLong())
    {
        nextState = &stateBrightness;
    }
    else if (buttonsTimedMultiple[getButtonIndex(&data)].isDoubleDownShortFinished())
    {
        nextState = &stateHue;
    }
    else
    {
        // intentionally empty
    }
    return *nextState;
}

void StateOn::deinit(DataType & data) const
{
    // intentionally empty
}


void StateBrightness::init(DataType & data) const
{
    // Change direction each time.
    data.incrementBrightness = !data.incrementBrightness;
}

Helpers::AbstractState<DataType> const & StateBrightness::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;
    if (buttonsTimedMultiple[getButtonIndex(&data)].isDownLong())
    {
        // As long as button stays down, modify brightness.
        if (data.incrementBrightness)
        {
            if ((255 - brightnessStep) >= displayColors[getLedIndex(&data)].brightness)
            {
                displayColors[getLedIndex(&data)].brightness += brightnessStep;
            }
        }
        else
        {
            if (brightnessStep <= displayColors[getLedIndex(&data)].brightness)
            {
                displayColors[getLedIndex(&data)].brightness -= brightnessStep;
            }
            // Prevent on and off state being indistinguishable.
            if (brightnessMin > displayColors[getLedIndex(&data)].brightness)
            {
                displayColors[getLedIndex(&data)].brightness = brightnessMin;
            }
        }
        updateDisplay = true;
    }
    else
    {
        // Fall back to stateOn once button is no longer pressed down.
        nextState = &stateOn;
    }
    return *nextState;
}

void StateBrightness::deinit(DataType & data) const
{
    settingsColors[getLedIndex(&data)].brightness = displayColors[getLedIndex(&data)].brightness;
    saveSettings = true;
}


void StateHue::init(DataType & data) const
{
    data.stateTimeout = DollHouse::durationStateTimeout;
}

Helpers::AbstractState<DataType> const & StateHue::process(DataType & data) const
{
    ButtonTimedProperties::Duration_t constexpr blinkPeriodicity = 20;
    ButtonTimedProperties::Duration_t const blinkSubperiod = (data.stateTimeout % blinkPeriodicity);
    switch (blinkSubperiod)
    {
    case 0:
    {
        displayColors[getLedIndex(&data)].brightness /= 2;
        updateDisplay = true;
        break;
    }
    case (blinkPeriodicity / 2):
    {
        displayColors[getLedIndex(&data)].brightness = settingsColors[getLedIndex(&data)].brightness;
        updateDisplay = true;
        break;
    }
    }


    Helpers::AbstractState<DataType> const * nextState = this;
    if (buttonsTimedMultiple[getButtonIndex(&data)].isSingleDownShortFinished())
    {
        // Change to next major hue.
        displayColors[getLedIndex(&data)].hue = Colors::SevenSegmentRgb::nextMajorHue(displayColors[getLedIndex(&data)].hue);

        updateDisplay = true;
        // Reset timeout while the user still interacts with this state - but retain subperiod for consistency of the blinking.
        data.stateTimeout = DollHouse::durationStateTimeout + (data.stateTimeout % blinkPeriodicity) - 1;
    }
    else if (buttonsTimedMultiple[getButtonIndex(&data)].isDownLong())
    {
        // Change hue continuously.
        displayColors[getLedIndex(&data)].hue += 5 * Colors::SevenSegmentRgb::singleDeltaHueUint16();

        updateDisplay = true;
        // Reset timeout while the user still interacts with this state - but retain subperiod for consistency of the blinking.
        data.stateTimeout = DollHouse::durationStateTimeout + (data.stateTimeout % blinkPeriodicity) - 1;
    }
    else if ((0 == data.stateTimeout) || buttonsTimedMultiple[getButtonIndex(&data)].isDoubleDownShortFinished())
    {
        nextState = &stateOn;
    }
    else
    {
        --data.stateTimeout;
    }
    return *nextState;
}

void StateHue::deinit(DataType & data) const
{
    displayColors[getLedIndex(&data)].brightness = settingsColors[getLedIndex(&data)].brightness;
    settingsColors[getLedIndex(&data)].hue = displayColors[getLedIndex(&data)].hue;
    saveSettings = true;
}


} // namespace DollHouse

