#include "dollHouseButtons.hpp"

namespace DollHouse
{

uint8_t buttonsMemory[1] = {0, };

bool buttonIsDoubleDownShortFinished(size_t const index)
{
    // TMP -> runtime translator.
    bool result = false;
    switch (index)
    {
    case 0:
    {
        result = DollHouse::Buttons<0>::isDoubleDownShortFinished();
        break;
    }
    case 1:
    {
        result = DollHouse::Buttons<1>::isDoubleDownShortFinished();
        break;
    }
    }
    static_assert(2 == DollHouse::numberOfButtons);
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
        result = DollHouse::Buttons<0>::isSingleDownShortFinished();
        break;
    }
    case 1:
    {
        result = DollHouse::Buttons<1>::isSingleDownShortFinished();
        break;
    }
    }
    static_assert(2 == DollHouse::numberOfButtons);
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
        result = DollHouse::Buttons<0>::isDownLong();
        break;
    }
    case 1:
    {
        result = DollHouse::Buttons<1>::isDownLong();
        break;
    }
    }
    static_assert(2 == DollHouse::numberOfButtons);
    return result;
}

} // namespace DollHouse

