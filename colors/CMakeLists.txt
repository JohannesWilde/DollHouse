
add_library(customColors
    colorCustom.hpp
    colorRgbw.cpp
    colorRgbw.hpp
    sevenSegmentRgb.cpp
    sevenSegmentRgb.hpp
)

target_include_directories(customColors
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)
