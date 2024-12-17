#ifndef COLORS_CUSTOM_HPP
#define COLORS_CUSTOM_HPP

namespace Colors
{

struct ColorCustom
{
    // All values are supposed to be in the range [0., 1.].
    float hue;
    float brightness;

    ColorCustom()
        : hue(0.)
        , brightness(0.)
    {
        // intentionally empty
    }

    ColorCustom(float const hueValue, float const brightnessValue)
        : hue(hueValue)
        , brightness(brightnessValue)
    {
        if (0. > brightness)
        {
            brightness = 0;
        }
        else if (1. < brightness)
        {
            brightness = 1.;
        }
        if (0. > hue)
        {
            hue = 0;
        }
        else if (1. < hue)
        {
            hue = 1.;
        }
    }
};

} // namespace Colors

#endif // COLORS_CUSTOM_HPP
