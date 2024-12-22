#include "colorRgbw.hpp"

namespace // anonymous namespace
{
	
inline uint8_t scaleColorPart(uint8_t const input, double const & scaleFactor)
{
    // this uses approximately 1200 bytes of flash storage!
	double const scaledValue = static_cast<double>(input) * scaleFactor;
	if (255. < scaledValue)
	{
		return 255;
	}
    else if (0. > scaledValue)
	{
		return 0;
	}
	else
	{
		return static_cast<uint8_t>(scaledValue);
	}
}

inline uint8_t addColorPart(uint8_t const one, uint8_t const two)
{
  if ((255 - one) < two)
	{
		return 255; // saturate if summation would overflow
	}
	else
	{
		return one + two;
	}
}

} // namespace anonymous

namespace Colors
{

ColorRgbw colorScaleBrightness(ColorRgbw const & input, double const & scaleFactor)
{
    return Colors::ColorRgbw(scaleColorPart(input.red   , scaleFactor),
                             scaleColorPart(input.green , scaleFactor),
                             scaleColorPart(input.blue  , scaleFactor),
                             scaleColorPart(input.white , scaleFactor));
}

ColorRgbw addColors(ColorRgbw const & one, ColorRgbw const & two)
{
    return Colors::ColorRgbw(addColorPart(one.red   , two.red),
                             addColorPart(one.green , two.green),
                             addColorPart(one.blue  , two.blue),
                             addColorPart(one.white , two.white));
}

}
