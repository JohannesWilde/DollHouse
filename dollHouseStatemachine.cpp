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

DataType dataTypes[numberOfButtons] = {
    {1, 0, },
    {2, 1, },
    {3, 2, },
    {4, 3, },
    {5, 4, },
    {6, 5, },
    {7, 6, },
    {8, 7, },
    };

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


void StateOff::init(DataType & data) const
{
    displayColors[data.ledIndex] = Colors::ColorCustomFixed(0, 0);
    updateDisplay = true;
}

Helpers::AbstractState<DataType> const & StateOff::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;
    if (buttonsTimedMultiple[data.buttonIndex].isSingleDownShortFinished())
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
    displayColors[data.ledIndex] = settingsColors[data.ledIndex];
    updateDisplay = true;
}

Helpers::AbstractState<DataType> const & StateOn::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;
    if (buttonsTimedMultiple[data.buttonIndex].isSingleDownShortFinished())
    {
        nextState = &stateOff;
    }
    else if (buttonsTimedMultiple[data.buttonIndex].isDownLong())
    {
        nextState = &stateBrightness;
    }
    else if (buttonsTimedMultiple[data.buttonIndex].isDoubleDownShortFinished())
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
    if (buttonsTimedMultiple[data.buttonIndex].isDownLong())
    {
        // As long as button stays down, modify brightness.
        static constexpr uint8_t brightnessStepSingle = 1;
        static constexpr uint8_t brightnessStep = 2 * brightnessStepSingle;
        static constexpr uint8_t brightnessMin = 2 * brightnessStepSingle;
        if (data.incrementBrightness)
        {
            if ((255 - brightnessStep) >= displayColors[data.ledIndex].brightness)
            {
                displayColors[data.ledIndex].brightness += brightnessStep;
            }
        }
        else
        {
            if (brightnessStep <= displayColors[data.ledIndex].brightness)
            {
                displayColors[data.ledIndex].brightness -= brightnessStep;
            }
            // Prevent on and off state being indistinguishable.
            if (brightnessMin > displayColors[data.ledIndex].brightness)
            {
                displayColors[data.ledIndex].brightness = brightnessMin;
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
    settingsColors[data.ledIndex].brightness = displayColors[data.ledIndex].brightness;
    saveSettings = true;
}


void StateHue::init(DataType & data) const
{
    data.stateTimeout = DollHouse::durationStateTimeout;
}

Helpers::AbstractState<DataType> const & StateHue::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;
    if (buttonsTimedMultiple[data.buttonIndex].isSingleDownShortFinished())
    {
        // Change to next major hue.
        displayColors[data.ledIndex].hue = Colors::SevenSegmentRgb::nextMajorHue(displayColors[data.ledIndex].hue);

        updateDisplay = true;
        // Reset timeout while the user still interacts with this state.
        data.stateTimeout = DollHouse::durationStateTimeout;
    }
    else if (buttonsTimedMultiple[data.buttonIndex].isDownLong())
    {
        // Change hue continuously.
        displayColors[data.ledIndex].hue += 5 * Colors::SevenSegmentRgb::singleDeltaHueUint16();

        updateDisplay = true;
        // Reset timeout while the user still interacts with this state.
        data.stateTimeout = DollHouse::durationStateTimeout;
    }
    else if ((0 == data.stateTimeout) || buttonsTimedMultiple[data.buttonIndex].isDoubleDownShortFinished())
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
    settingsColors[data.ledIndex] = displayColors[data.ledIndex];
    saveSettings = true;
}


} // namespace DollHouse

