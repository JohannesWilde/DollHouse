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

    // loop

    for (DollHouse::CustomButtomTimedMultiple & button : DollHouse::buttonsTimedMultiple)
    {
        button.update();
    }

    for (size_t index = 0; index < DollHouse::numberOfButtons; ++index)
    {
        DollHouse::statemachines[index].process(DollHouse::dataTypes[index]);
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

    std::cout << "The End." << std::endl;
}
