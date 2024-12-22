
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
    PRIVATE dollHouseStatemachine
)


add_executable(testColorsCustom
    testColorsCustom.cpp
)

target_link_libraries(testColorsCustom
    PRIVATE customColors
)
