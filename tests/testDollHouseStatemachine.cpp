#include <colors/colorCustom.hpp>
#include <colors/sevenSegmentRgb.hpp>

#include <helpers/statemachine.hpp>
#include <helpers/tmpLoop.hpp>

#include <dollHouseButtons.hpp>

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string.h>



// Statemachine

struct DataType
{
    Colors::ColorCustom & settingsColor;
    bool & saveSettings;
    Colors::ColorCustom & displayColor;
    bool & updateDisplay;
    size_t const buttonIndex;
    bool incrementBrightness = false;
    ButtonTimedProperties::Duration_t stateTimeout = 0;
};

class StateOff : public Helpers::AbstractState<DataType>
{
    void init(DataType & data) const override;
    AbstractState const & process(DataType & data) const override;
    void deinit(DataType & data) const override;
};

static StateOff const stateOff;

class StateOn : public Helpers::AbstractState<DataType>
{
    void init(DataType & data) const override;
    AbstractState const & process(DataType & data) const override;
    void deinit(DataType & data) const override;
};

static StateOn const stateOn;

class StateBrightness : public Helpers::AbstractState<DataType>
{
    void init(DataType & data) const override;
    AbstractState const & process(DataType & data) const override;
    void deinit(DataType & data) const override;
};

static StateBrightness const stateBrightness;

class StateHue : public Helpers::AbstractState<DataType>
{
    void init(DataType & data) const override;
    AbstractState const & process(DataType & data) const override;
    void deinit(DataType & data) const override;
};

static StateHue const stateHue;


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


int main(int argc, char* argv[])
{
    // initialize
    static_assert(0 < sizeof(DollHouse::buttonsMemory));
    memset(DollHouse::buttonsMemory, 0, sizeof(DollHouse::buttonsMemory) / sizeof(DollHouse::buttonsMemory[0]));
    Helpers::TMP::Loop<DollHouse::numberOfButtons, DollHouse::WrapperInitialize>::impl();

    // variables
    static Colors::ColorCustom settingsColors[DollHouse::numberOfButtons] = {};
    static bool saveSettings = false;
    static Colors::ColorCustom displayColors[DollHouse::numberOfButtons] = {};
    static bool updateDisplay = true;

    // load settings from EEPROM
    // todo
    for (size_t index = 0; index < DollHouse::numberOfButtons; ++index)
    {
        settingsColors[index] = Colors::ColorCustom(1.0, 1.0);
    }

    // statemachine
    DataType dataTypes[DollHouse::numberOfButtons] = {
        {settingsColors[0], saveSettings, displayColors[0], updateDisplay, 0, },
        {settingsColors[1], saveSettings, displayColors[1], updateDisplay, 1, },
    };

    Helpers::Statemachine<DataType> statemachines[DollHouse::numberOfButtons] = {
        Helpers::Statemachine(stateOff),
        Helpers::Statemachine(stateOff),
    };

    // loop
    Helpers::TMP::Loop<DollHouse::numberOfButtons, DollHouse::WrapperUpdate>::impl();

    for (size_t index = 0; index < DollHouse::numberOfButtons; ++index)
    {
        statemachines[index].process(dataTypes[index]);
    }

    if (saveSettings)
    {
        // todo
    }

    if (updateDisplay)
    {
        // todo
        // convert Colors::ColorCustom to RGB
        // show NEO-pixels
    }

    assert(true);


    // deinitialize
    Helpers::TMP::Loop<DollHouse::numberOfButtons, DollHouse::WrapperDeinitialize>::impl();

    std::cout << "The End." << std::endl;
}
