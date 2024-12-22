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
        {1, 0, },
        {2, 1, },
        {3, 2, },
        {4, 3, },
        {5, 4, },
        {6, 5, },
        {7, 6, },
        {8, 7, },
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
