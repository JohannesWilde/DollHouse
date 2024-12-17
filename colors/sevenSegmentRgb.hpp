#ifndef SEVEN_SEGMENT_RGB
#define SEVEN_SEGMENT_RGB

namespace Colors
{

namespace SevenSegmentRgb
{

// Hue -> RGB mapping
//   0/7      1/7      2/7      3/7      4/7      5/7      6/7      7/7
//    |--------|--------|--------|--------|--------|--------|--------|
//  R+B+G     R+B       B       G+B       G       R+G       R      R+B+G
//  white    violet   blue   turquoise  green   yellow     red     white

float nextMajorHue(float const hue);

constexpr float singleDeltaHue()
{
    return  1.f / (7.f * 256.f);
}

} // namespace SevenSegmentRgb

} // namespace Colors

#endif // SEVEN_SEGMENT_RGB
