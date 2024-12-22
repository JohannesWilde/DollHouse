#ifndef COLORS_CUSTOM_FIXED_HPP
#define COLORS_CUSTOM_FIXED_HPP

#include <stdint.h>

namespace Colors
{

struct ColorCustomFixed
{
    // All values are supposed to be in the range [0., 1.].
    uint16_t hue;
    uint8_t brightness;

    ColorCustomFixed()
        : hue(0)
        , brightness(0)
    {
        // intentionally empty
    }

    ColorCustomFixed(uint16_t const hueValue, uint8_t const brightnessValue)
        : hue(hueValue)
        , brightness(brightnessValue)
    {
        // intentionally empty
    }
};

} // namespace Colors

#endif // COLORS_CUSTOM_FIXED_HPP
