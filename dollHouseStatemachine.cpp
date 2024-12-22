#include "dollHouseStatemachine.hpp"

#include "dollHouseButtons.hpp"


namespace DollHouse
{

Colors::ColorCustomFixed settingsColors[ledsCount] = {};
bool saveSettings = false;
Colors::ColorCustomFixed displayColors[ledsCount] = {};
bool updateDisplay = false;

StateOff const stateOff;
StateOn const stateOn;
StateBrightness const stateBrightness;
StateHue const stateHue;

void StateOff::init(DataType & data) const
{
    displayColors[data.ledIndex] = Colors::ColorCustomFixed(0, 0);
    updateDisplay = true;
}

Helpers::AbstractState<DataType> const & StateOff::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;
    if (DollHouse::buttonIsSingleDownShortFinished(data.buttonIndex))
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
    if (DollHouse::buttonIsSingleDownShortFinished(data.buttonIndex))
    {
        nextState = &stateOff;
    }
    else if (DollHouse::buttonIsDownLong(data.buttonIndex))
    {
        nextState = &stateBrightness;
    }
    else if (DollHouse::buttonIsDoubleDownShortFinished(data.buttonIndex))
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
    if (DollHouse::buttonIsDownLong(data.buttonIndex))
    {
        // As long as button stays down, modify brightness.
        static constexpr uint8_t brightnessStepSingle = 1;
        static constexpr uint8_t brightnessStep = 2 * brightnessStepSingle;
        static constexpr uint8_t brightnessMin = 2 * brightnessStepSingle;
        if (data.incrementBrightness)
        {
            if (255 != displayColors[data.ledIndex].brightness)
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
    if (DollHouse::buttonIsSingleDownShortFinished(data.buttonIndex))
    {
        // Change to next major hue.
        displayColors[data.ledIndex].hue = Colors::SevenSegmentRgb::nextMajorHue(displayColors[data.ledIndex].hue);

        updateDisplay = true;
        // Reset timeout while the user still interacts with this state.
        data.stateTimeout = DollHouse::durationStateTimeout;
    }
    else if (DollHouse::buttonIsDownLong(data.buttonIndex))
    {
        // Change hue continuously.
        displayColors[data.ledIndex].hue += 5 * Colors::SevenSegmentRgb::singleDeltaHueUint16();

        updateDisplay = true;
        // Reset timeout while the user still interacts with this state.
        data.stateTimeout = DollHouse::durationStateTimeout;
    }
    else if ((0 == data.stateTimeout) || DollHouse::buttonIsDoubleDownShortFinished(data.buttonIndex))
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

