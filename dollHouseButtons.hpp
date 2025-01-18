#ifndef DOLL_HOUSE_BUTTONS_HPP
#define DOLL_HOUSE_BUTTONS_HPP

#include "ArduinoDrivers/button.hpp"
#include "ArduinoDrivers/buttonDynamic.hpp"
#include "ArduinoDrivers/buttonTimedMultiple.hpp"
#include "ArduinoDrivers/simplePinBit.hpp"

namespace DollHouse
{

constexpr SimplePin::State downState = SimplePin::State::Zero;
constexpr SimplePin::State upState = SimplePin::State::One;
constexpr ButtonTimedProperties::Duration_t durationShort = 2; // 50ms per cycle
constexpr ButtonTimedProperties::Duration_t durationLong = 8;
constexpr ButtonTimedProperties::Duration_t durationCombineMax = 6;
constexpr ButtonTimedProperties::Duration_t durationStateTimeout = 100;

extern uint8_t buttonsMemory[1];


static size_t constexpr numberOfButtons = 8;

typedef SimplePinBit<0, buttonsMemory> Pin0;
typedef SimplePinBit<1, buttonsMemory> Pin1;
typedef SimplePinBit<2, buttonsMemory> Pin2;
typedef SimplePinBit<3, buttonsMemory> Pin3;
typedef SimplePinBit<4, buttonsMemory> Pin4;
typedef SimplePinBit<5, buttonsMemory> Pin5;
typedef SimplePinBit<6, buttonsMemory> Pin6;
typedef SimplePinBit<7, buttonsMemory> Pin7;

typedef Button<Pin0, downState> Button0;
typedef Button<Pin1, downState> Button1;
typedef Button<Pin2, downState> Button2;
typedef Button<Pin3, downState> Button3;
typedef Button<Pin4, downState> Button4;
typedef Button<Pin5, downState> Button5;
typedef Button<Pin6, downState> Button6;
typedef Button<Pin7, downState> Button7;


void buttonInitialize(void const * const instance);
void buttonDeinitialize(void const * const instance);
bool buttonIsDown(void const * const instance);


typedef ButtonDynamic<&buttonInitialize, &buttonIsDown, &buttonDeinitialize> CustomButton;
typedef ButtonTimedMultiple<CustomButton, durationShort, durationLong, durationCombineMax, 5> CustomButtomTimedMultiple;

extern CustomButtomTimedMultiple buttonsTimedMultiple[numberOfButtons];

} // namespace DollHouse

#endif // DOLL_HOUSE_BUTTONS_HPP
