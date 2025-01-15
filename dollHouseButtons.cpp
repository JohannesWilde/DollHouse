#include "dollHouseButtons.hpp"

namespace DollHouse
{

uint8_t buttonsMemory[1] = {0, };


void buttonInitialize(uint8_t const index)
{
    switch (index)
    {
    case 0: return Button0::initialize();
    case 1: return Button1::initialize();
    case 2: return Button2::initialize();
    case 3: return Button3::initialize();
    case 4: return Button4::initialize();
    case 5: return Button5::initialize();
    case 6: return Button6::initialize();
    case 7: return Button7::initialize();
    }
    // assert(numberOfButtons > index);
    static_assert(8 == numberOfButtons, "buttonInitialize assumes different amount of buttons.");
}

void buttonDeinitialize(uint8_t const index)
{
    switch (index)
    {
    case 0: return Button0::deinitialize();
    case 1: return Button1::deinitialize();
    case 2: return Button2::deinitialize();
    case 3: return Button3::deinitialize();
    case 4: return Button4::deinitialize();
    case 5: return Button5::deinitialize();
    case 6: return Button6::deinitialize();
    case 7: return Button7::deinitialize();
    }
    // assert(numberOfButtons > index);
    static_assert(8 == numberOfButtons, "buttonDeinitialize assumes different amount of buttons.");
}

bool buttonIsDown(uint8_t const index)
{
    switch (index)
    {
    case 0: return Button0::isDown();
    case 1: return Button1::isDown();
    case 2: return Button2::isDown();
    case 3: return Button3::isDown();
    case 4: return Button4::isDown();
    case 5: return Button5::isDown();
    case 6: return Button6::isDown();
    case 7: return Button7::isDown();
    }
    // assert(numberOfButtons > index);
    static_assert(8 == numberOfButtons, "buttonIsDown assumes different amount of buttons.");
    return false;
}

CustomButtomTimedMultiple buttonsTimedMultiple[] = {
    CustomButtomTimedMultiple(0),
    CustomButtomTimedMultiple(1),
    CustomButtomTimedMultiple(2),
    CustomButtomTimedMultiple(3),
    CustomButtomTimedMultiple(4),
    CustomButtomTimedMultiple(5),
    CustomButtomTimedMultiple(6),
    CustomButtomTimedMultiple(7),
};

} // namespace DollHouse

