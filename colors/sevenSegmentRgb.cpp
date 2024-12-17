#include "sevenSegmentRgb.hpp"

#include <cmath>


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

} // namespace SevenSegmentRgb

} // namespace Colors
