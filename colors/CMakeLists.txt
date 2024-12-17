
add_library(customColors
    colorCustom.hpp
    sevenSegmentRgb.cpp
    sevenSegmentRgb.hpp
)

target_include_directories(customColors
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..
)
