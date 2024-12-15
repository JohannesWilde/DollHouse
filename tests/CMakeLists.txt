cmake_minimum_required(VERSION 3.19)

project(DollHouseTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(../ArduinoDrivers ArduinoDrivers)
add_subdirectory(../helpers helpers)


add_executable(testButtonTimeMultiple
    testButtonTimeMultiple.cpp
)

target_link_libraries(testButtonTimeMultiple
    PRIVATE arduinoDrivers
    PRIVATE helpers
)


add_executable(testDollHouseStatemachine
    testDollHouseStatemachine.cpp
)

target_link_libraries(testDollHouseStatemachine
    PRIVATE arduinoDrivers
    PRIVATE helpers
)
