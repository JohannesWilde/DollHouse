#ifndef COLOR_RGBW_HPP
#define COLOR_RGBW_HPP

#include <stdint.h>

namespace Colors
{

class ColorRgbw
{
public:
    constexpr ColorRgbw()
        : red(0)
        , green(0)
        , blue(0)
        , white(0)
    {
    }

    constexpr ColorRgbw(uint8_t const red,
                        uint8_t const green,
                        uint8_t const blue,
                        uint8_t const white = 0)
        : red(red)
        , green(green)
        , blue(blue)
        , white(white)
    {
    }

    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t white;
};

// Scale components independently to range [0, 255]. Outliers will be clipped.
ColorRgbw colorScaleBrightness(ColorRgbw const & input, uint8_t const scaleFactor);
ColorRgbw colorScaleBrightness(ColorRgbw const & input, double const & scaleFactor);

// Sum up components of colors independently. Saturates at 0xff for each component.
ColorRgbw addColors(ColorRgbw const & one, ColorRgbw const & two);

ColorRgbw constexpr Black     = ColorRgbw(0, 0, 0);
ColorRgbw constexpr Red       = ColorRgbw(255, 0, 0);
ColorRgbw constexpr Green     = ColorRgbw(0, 255, 0);
ColorRgbw constexpr Blue      = ColorRgbw(0, 0, 255);
ColorRgbw constexpr Yellow    = ColorRgbw(255, 246, 0);
ColorRgbw constexpr White     = ColorRgbw(255, 255, 255);
ColorRgbw constexpr Gray      = ColorRgbw(60, 60, 60);

namespace WhithWhite
{

ColorRgbw constexpr White     = ColorRgbw(0, 0, 0, 255);
ColorRgbw constexpr Gray      = ColorRgbw(0, 0, 0, 60);

} // namespace WhithWhite

} // namespace Colors

#endif // COLOR_RGBW_HPP
