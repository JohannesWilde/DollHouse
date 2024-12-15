cmake_minimum_required(VERSION 3.19)

project(DollHouseTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(../ArduinoDrivers ArduinoDrivers)
add_subdirectory(../helpers helpers)


add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE arduinoDrivers
    PRIVATE helpers
)
