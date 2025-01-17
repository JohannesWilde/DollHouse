#ifndef DOLL_HOUSE_STATEMACHINE_HPP
#define DOLL_HOUSE_STATEMACHINE_HPP

#include "dollHouseButtons.hpp"

#include "ArduinoDrivers/buttonTimed.hpp"

#include "colors/colorCustomFixed.hpp"
#include "colors/sevenSegmentRgb.hpp"

#include "helpers/statemachine.hpp"
#include "helpers/tmpLoop.hpp"


namespace DollHouse
{

struct DataType
{
    bool incrementBrightness = false;
    ButtonTimedProperties::Duration_t stateTimeout = 0;
};

class StateOff : public Helpers::AbstractState<DataType>
{
    void init(DataType & data) const override;
    AbstractState const & process(DataType & data) const override;
    void deinit(DataType & data) const override;
};

extern StateOff const stateOff;

class StateOn : public Helpers::AbstractState<DataType>
{
    void init(DataType & data) const override;
    AbstractState const & process(DataType & data) const override;
    void deinit(DataType & data) const override;
};

extern StateOn const stateOn;

class StateBrightness : public Helpers::AbstractState<DataType>
{
    void init(DataType & data) const override;
    AbstractState const & process(DataType & data) const override;
    void deinit(DataType & data) const override;
};

extern StateBrightness const stateBrightness;

class StateHue : public Helpers::AbstractState<DataType>
{
    void init(DataType & data) const override;
    AbstractState const & process(DataType & data) const override;
    void deinit(DataType & data) const override;
};

extern StateHue const stateHue;


uint8_t constexpr ledsCount = 9; // todo: move to config-header

extern Colors::ColorCustomFixed settingsColors[ledsCount];
extern bool saveSettings;
extern Colors::ColorCustomFixed displayColors[ledsCount];
extern bool updateDisplay;

extern DataType dataTypes[DollHouse::numberOfButtons];
extern Helpers::Statemachine<DollHouse::DataType> statemachines[DollHouse::numberOfButtons];

} // namespace DollHouse

#endif // DOLL_HOUSE_STATEMACHINE_HPP
