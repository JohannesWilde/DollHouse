#include "dollHouseStatemachine.hpp"

#include "dollHouseButtons.hpp"


namespace DollHouse
{

StateOff const stateOff;
StateOn const stateOn;
StateBrightness const stateBrightness;
StateHue const stateHue;

void StateOff::init(DataType & data) const
{
    data.displayColor = Colors::ColorCustom(0., 0.);
    data.updateDisplay = true;
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
    data.displayColor = data.settingsColor;
    data.updateDisplay = true;
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
        static constexpr float brightnessStep = 1./256.;
        if (data.incrementBrightness)
        {
            float const newBrightness = data.displayColor.brightness + brightnessStep;
            if (1. < newBrightness)
            {
                data.displayColor.brightness = 1.;
            }
            else
            {
                data.displayColor.brightness = newBrightness;
            }
        }
        else
        {
            float const newBrightness = data.displayColor.brightness - brightnessStep;
            if (0. > newBrightness)
            {
                data.displayColor.brightness = 0.;
            }
            else
            {
                data.displayColor.brightness = newBrightness;
            }
        }
        data.updateDisplay = true;
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
    data.settingsColor = data.displayColor;
    data.saveSettings = true;
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
        data.displayColor.hue = Colors::SevenSegmentRgb::nextMajorHue(data.displayColor.hue);

        data.updateDisplay = true;
        // Reset timeout while the user still interacts with this state.
        data.stateTimeout = DollHouse::durationStateTimeout;
    }
    else if (DollHouse::buttonIsDownLong(data.buttonIndex))
    {
        // Change hue continuously.
        float nextHue = data.displayColor.hue + 5.f * Colors::SevenSegmentRgb::singleDeltaHue();
        if (1. <= nextHue)
        {
            nextHue -= 1.;
        }
        data.displayColor.hue = nextHue;

        data.updateDisplay = true;
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
    data.settingsColor = data.displayColor;
    data.saveSettings = true;
}


} // namespace DollHouse

