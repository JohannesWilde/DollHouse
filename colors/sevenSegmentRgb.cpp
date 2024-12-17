#include "sevenSegmentRgb.hpp"

#include <math.h>


namespace Colors
{

namespace SevenSegmentRgb
{

// Hue -> RGB mapping
//   0/7      1/7      2/7      3/7      4/7      5/7      6/7      7/7
//    |--------|--------|--------|--------|--------|--------|--------|
//  R+B+G     R+B       B       G+B       G       R+G       R      R+B+G
//  white    violet   blue   turquoise  green   yellow     red     white

float nextMajorHue(float const hue)
{
    float nextMajorHue = (floor(hue * 7.f) + 1.f) / 7.f;
    if (1.f <= nextMajorHue)
    {
        nextMajorHue -= 1.f;
    }
    return nextMajorHue;
}

Colors::ColorRgbw toRgb(Colors::ColorCustom const & color)
{
    ColorRgbw newColor;

    float const scaledHue = color.hue * 7.;
    uint8_t const hueBase = static_cast<int>(scaledHue);
    uint8_t const integerDiff = (scaledHue - static_cast<float>(hueBase)) * 255;

    switch (hueBase)
    {
    case 7:
        // fall through
    case 0:
    {
        newColor = ColorRgbw(255, 255, 255);
        newColor.green -= integerDiff;
        break;
    }
    case 1:
    {
        newColor = ColorRgbw(255, 0, 255);
        newColor.red -= integerDiff;
        break;
    }
    case 2:
    {
        newColor = ColorRgbw(0, 0, 255);
        newColor.green = integerDiff;
        break;
    }
    case 3:
    {
        newColor = ColorRgbw(0, 255, 255);
        newColor.blue -= integerDiff;
        break;
    }
    case 4:
    {
        newColor = ColorRgbw(0, 255, 0);
        newColor.red = integerDiff;
        break;
    }
    case 5:
    {
        newColor = ColorRgbw(255, 255, 0);
        newColor.green -= integerDiff;
        break;
    }
    case 6:
    {
        newColor = ColorRgbw(255, 0, 0);
        newColor.blue = integerDiff;
        newColor.green = integerDiff;
        break;
    }
    default:
    {
        // intentionally empty
    }
    }

    return newColor;
}

} // namespace SevenSegmentRgb

} // namespace Colors
