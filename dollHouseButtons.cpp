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
    case 2:
    {
        result = DollHouse::Buttons<2>::isDoubleDownShortFinished();
        break;
    }
    case 3:
    {
        result = DollHouse::Buttons<3>::isDoubleDownShortFinished();
        break;
    }
    case 4:
    {
        result = DollHouse::Buttons<4>::isDoubleDownShortFinished();
        break;
    }
    case 5:
    {
        result = DollHouse::Buttons<5>::isDoubleDownShortFinished();
        break;
    }
    case 6:
    {
        result = DollHouse::Buttons<6>::isDoubleDownShortFinished();
        break;
    }
    case 7:
    {
        result = DollHouse::Buttons<7>::isDoubleDownShortFinished();
        break;
    }
    }
    static_assert(8 == DollHouse::numberOfButtons);
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
    case 2:
    {
        result = DollHouse::Buttons<2>::isSingleDownShortFinished();
        break;
    }
    case 3:
    {
        result = DollHouse::Buttons<3>::isSingleDownShortFinished();
        break;
    }
    case 4:
    {
        result = DollHouse::Buttons<4>::isSingleDownShortFinished();
        break;
    }
    case 5:
    {
        result = DollHouse::Buttons<5>::isSingleDownShortFinished();
        break;
    }
    case 6:
    {
        result = DollHouse::Buttons<6>::isSingleDownShortFinished();
        break;
    }
    case 7:
    {
        result = DollHouse::Buttons<7>::isSingleDownShortFinished();
        break;
    }
    }
    static_assert(8 == DollHouse::numberOfButtons);
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
    case 2:
    {
        result = DollHouse::Buttons<2>::isDownLong();
        break;
    }
    case 3:
    {
        result = DollHouse::Buttons<3>::isDownLong();
        break;
    }
    case 4:
    {
        result = DollHouse::Buttons<4>::isDownLong();
        break;
    }
    case 5:
    {
        result = DollHouse::Buttons<5>::isDownLong();
        break;
    }
    case 6:
    {
        result = DollHouse::Buttons<6>::isDownLong();
        break;
    }
    case 7:
    {
        result = DollHouse::Buttons<7>::isDownLong();
        break;
    }
    }
    static_assert(8 == DollHouse::numberOfButtons);
    return result;
}

} // namespace DollHouse

