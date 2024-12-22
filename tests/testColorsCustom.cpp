#include <colors/sevenSegmentRgb.hpp>

#include <cassert>
#include <iostream>

int main(int argc, char* argv[])
{
    Colors::ColorCustom const inputColor(0., 0.);

    Colors::ColorRgbw const outputColor = Colors::SevenSegmentRgb::toRgb(inputColor);

    assert((0 == outputColor.red) && (0 == outputColor.green) && (0 == outputColor.blue));

    std::cout << "testColorsCustom succeeded." << std::endl;
}
