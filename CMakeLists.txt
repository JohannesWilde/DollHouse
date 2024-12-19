cmake_minimum_required(VERSION 3.19)

project(DollHouse)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(ArduinoDrivers)
add_subdirectory(colors)
add_subdirectory(helpers)


add_library(dollHouseStatemachine
    dollHouseButtons.cpp
    dollHouseButtons.hpp
)

target_link_libraries(dollHouseStatemachine
    PUBLIC arduinoDrivers
    PUBLIC customColors
    PUBLIC helpers
)

target_include_directories(dollHouseStatemachine
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)


add_subdirectory(tests)
