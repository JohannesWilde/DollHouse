#include <colors/sevenSegmentRgb.hpp>

#include <cassert>
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
    std::vector<Colors::ColorCustom> const inputColors{{0., 0.}, {0., 1.}, };
    std::vector<Colors::ColorRgbw> const expectedColors{{0, 0, 0}, {255, 255, 255}, };

    assert(inputColors.size() == expectedColors.size());

    for (size_t index = 0; inputColors.size() > index; ++index)
    {
        Colors::ColorRgbw const outputColor = Colors::SevenSegmentRgb::toRgb(inputColors[index]);

        assert((expectedColors[index].red == outputColor.red) &&
               (expectedColors[index].green == outputColor.green) &&
               (expectedColors[index].blue == outputColor.blue));
    }

    std::cout << "testColorsCustom succeeded." << std::endl;
}
