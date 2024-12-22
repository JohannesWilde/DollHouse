#include <dollHouseButtons.hpp>
#include <dollHouseStatemachine.hpp>

#include <cassert>
#include <iostream>
#include <string.h>


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
    DollHouse::DataType dataTypes[DollHouse::numberOfButtons] = {
        {settingsColors[0], saveSettings, displayColors[0], updateDisplay, 0, },
        {settingsColors[1], saveSettings, displayColors[1], updateDisplay, 1, },
        {settingsColors[2], saveSettings, displayColors[2], updateDisplay, 2, },
        {settingsColors[3], saveSettings, displayColors[3], updateDisplay, 3, },
        {settingsColors[4], saveSettings, displayColors[4], updateDisplay, 4, },
        {settingsColors[5], saveSettings, displayColors[5], updateDisplay, 5, },
        {settingsColors[6], saveSettings, displayColors[6], updateDisplay, 6, },
        {settingsColors[7], saveSettings, displayColors[7], updateDisplay, 7, },
    };

    Helpers::Statemachine<DollHouse::DataType> statemachines[DollHouse::numberOfButtons] = {
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
        Helpers::Statemachine(DollHouse::stateOff),
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
