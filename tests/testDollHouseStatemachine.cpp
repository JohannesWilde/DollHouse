
#include <ArduinoDrivers/button.hpp>
#include <ArduinoDrivers/buttonTimedMultiple.hpp>
#include <ArduinoDrivers/simplePinBit.hpp>

#include <helpers/statemachine.hpp>
#include <helpers/tmpLoop.hpp>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string.h>

static constexpr SimplePin::State downState = SimplePin::State::Zero;
static constexpr SimplePin::State upState = SimplePin::State::One;
static constexpr ButtonTimedProperties::Duration_t durationShort = 2; // 50ms per cycle
static constexpr ButtonTimedProperties::Duration_t durationLong = 8;
static constexpr ButtonTimedProperties::Duration_t durationCombineMax = 6;
static constexpr ButtonTimedProperties::Duration_t durationStateTimeout = 100;

static uint8_t buttonsMemory[1];

static size_t constexpr numberOfButtons = 2;

typedef SimplePinBit<0, buttonsMemory> Pin0;
typedef SimplePinBit<1, buttonsMemory> Pin1;

typedef Button<Pin0, downState> Button0;
typedef Button<Pin1, downState> Button1;

typedef ButtonTimedMultiple<Button0, durationShort, durationLong, durationCombineMax> ButtonTimedMultiple0;
typedef ButtonTimedMultiple<Button1, durationShort, durationLong, durationCombineMax> ButtonTimedMultiple1;

template <uint8_t index>
class Buttons;
template <> class Buttons<0> : public ButtonTimedMultiple0 {/* intentionally empty */};
template <> class Buttons<1> : public ButtonTimedMultiple1 {/* intentionally empty */};

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
struct WrapperUpdate
{
    static void impl()
    {
        Buttons<Index>::update();
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

bool buttonIsDoubleDownShortFinished(size_t const index)
{
    // TMP -> runtime translator.
    bool result = false;
    switch (index)
    {
    case 0:
    {
        result = Buttons<0>::isDoubleDownShortFinished();
        break;
    }
    case 1:
    {
        result = Buttons<1>::isDoubleDownShortFinished();
        break;
    }
    }
    static_assert(2 == numberOfButtons);
    return result;
}

bool buttonIsSingleDownShortFinished(size_t const index)
{
    // TMP -> runtime translator.
    bool result = false;
    switch (index)
    {
    case 0:
    {
        result = Buttons<0>::isSingleDownShortFinished();
        break;
    }
    case 1:
    {
        result = Buttons<1>::isSingleDownShortFinished();
        break;
    }
    }
    static_assert(2 == numberOfButtons);
    return result;
}

bool buttonIsDownLong(size_t const index)
{
    // TMP -> runtime translator.
    bool result = false;
    switch (index)
    {
    case 0:
    {
        result = Buttons<0>::isDownLong();
        break;
    }
    case 1:
    {
        result = Buttons<1>::isDownLong();
        break;
    }
    }
    static_assert(2 == numberOfButtons);
    return result;
}


// Statemachine

struct ColorCustom
{
    // All values are supposed to be in the range [0., 1.].
    float hue;
    float brightness;

    // Hue -> RGB mapping
    //    0       1/7      2/7      3/7      4/7      5/7      6/7      7/7
    //    |--------|--------|--------|--------|--------|--------|--------|
    //  R+B+G     R+B       B       G+B       G       R+G       R      R+B+G
    //  white    violet   blue   turquoise  green   yellow     red     white


    ColorCustom()
        : hue(0.)
        , brightness(0.)
    {
        // intentionally empty
    }

    ColorCustom(float const hueValue, float const brightnessValue)
        : hue(hueValue)
        , brightness(brightnessValue)
    {
        if (0. > brightness)
        {
            brightness = 0;
        }
        else if (1. < brightness)
        {
            brightness = 1.;
        }
        if (0. > hue)
        {
            hue = 0;
        }
        else if (1. < hue)
        {
            hue = 1.;
        }
    }
};

namespace SevenSegmentRgb
{

constexpr float nextMajorHue(float const hue)
{
    float nextMajorHue = (floor(hue * 7.f) + 1.f) / 7.f;
    if (1.f <= nextMajorHue)
    {
        nextMajorHue -= 1.f;
    }
    return nextMajorHue;
}

constexpr float singleDeltaHue()
{
    return  1.f / (7.f * 256.f);
}

} // namespace SevenSegmentRgb

struct DataType
{
    ColorCustom & settingsColor;
    bool & saveSettings;
    ColorCustom & displayColor;
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
    data.displayColor = ColorCustom(0., 0.);
    data.updateDisplay = true;
}

Helpers::AbstractState<DataType> const & StateOff::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;
    if (buttonIsSingleDownShortFinished(data.buttonIndex))
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
    if (buttonIsSingleDownShortFinished(data.buttonIndex))
    {
        nextState = &stateOff;
    }
    else if (buttonIsDownLong(data.buttonIndex))
    {
        nextState = &stateBrightness;
    }
    else if (buttonIsDoubleDownShortFinished(data.buttonIndex))
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
    if (buttonIsDownLong(data.buttonIndex))
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
    data.stateTimeout = durationStateTimeout;
}

Helpers::AbstractState<DataType> const & StateHue::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;
    if (buttonIsSingleDownShortFinished(data.buttonIndex))
    {
        // Change to next major hue.
        data.displayColor.hue = SevenSegmentRgb::nextMajorHue(data.displayColor.hue);

        data.updateDisplay = true;
        // Reset timeout while the user still interacts with this state.
        data.stateTimeout = durationStateTimeout;
    }
    else if (buttonIsDownLong(data.buttonIndex))
    {
        // Change hue continuously.
        float nextHue = data.displayColor.hue + 3.f * SevenSegmentRgb::singleDeltaHue();
        if (1. <= nextHue)
        {
            nextHue -= 1.;
        }
        data.displayColor.hue = nextHue;

        data.updateDisplay = true;
        // Reset timeout while the user still interacts with this state.
        data.stateTimeout = durationStateTimeout;
    }
    else if ((0 == data.stateTimeout) || buttonIsDoubleDownShortFinished(data.buttonIndex))
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
    static_assert(0 < sizeof(buttonsMemory));
    memset(buttonsMemory, 0, sizeof(buttonsMemory) / sizeof(buttonsMemory[0]));
    Helpers::TMP::Loop<numberOfButtons, WrapperInitialize>::impl();

    // variables
    static ColorCustom settingsColors[numberOfButtons] = {};
    static bool saveSettings = false;
    static ColorCustom displayColors[numberOfButtons] = {};
    static bool updateDisplay = true;

    // load settings from EEPROM
    // todo
    for (size_t index = 0; index < numberOfButtons; ++index)
    {
        settingsColors[index] = ColorCustom(1.0, 1.0);
    }

    // statemachine
    DataType dataTypes[numberOfButtons] = {
        {settingsColors[0], saveSettings, displayColors[0], updateDisplay, 0, },
        {settingsColors[1], saveSettings, displayColors[1], updateDisplay, 1, },
    };

    Helpers::Statemachine<DataType> statemachines[numberOfButtons] = {
        Helpers::Statemachine(stateOff),
        Helpers::Statemachine(stateOff),
    };

    // loop
    Helpers::TMP::Loop<numberOfButtons, WrapperUpdate>::impl();

    for (size_t index = 0; index < numberOfButtons; ++index)
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
        // convert ColorCustom to RGB
        // show NEO-pixels
    }

    assert(true);


    // deinitialize
    Helpers::TMP::Loop<numberOfButtons, WrapperDeinitialize>::impl();

    std::cout << "The End." << std::endl;
}
