#ifndef DOLL_HOUSE_BUTTONS_HPP
#define DOLL_HOUSE_BUTTONS_HPP

#include <ArduinoDrivers/button.hpp>
#include <ArduinoDrivers/buttonTimedMultiple.hpp>
#include <ArduinoDrivers/simplePinBit.hpp>

namespace DollHouse
{

constexpr SimplePin::State downState = SimplePin::State::Zero;
constexpr SimplePin::State upState = SimplePin::State::One;
constexpr ButtonTimedProperties::Duration_t durationShort = 2; // 50ms per cycle
constexpr ButtonTimedProperties::Duration_t durationLong = 8;
constexpr ButtonTimedProperties::Duration_t durationCombineMax = 6;
constexpr ButtonTimedProperties::Duration_t durationStateTimeout = 100;

extern uint8_t buttonsMemory[1];


static size_t constexpr numberOfButtons = 2;

typedef SimplePinBit<0, buttonsMemory> Pin0;
typedef SimplePinBit<1, buttonsMemory> Pin1;

typedef Button<Pin0, downState> Button0;
typedef Button<Pin1, downState> Button1;

typedef ButtonTimedMultiple<Button0, durationShort, durationLong, durationCombineMax> ButtonTimedMultiple0;
typedef ButtonTimedMultiple<Button1, durationShort, durationLong, durationCombineMax> ButtonTimedMultiple1;

template <uint8_t index>
class Buttons;
template <> class Buttons<0> : public ButtonTimedMultiple0 {/* intentionally empty */};
template <> class Buttons<1> : public ButtonTimedMultiple1 {/* intentionally empty */};


// Wrappers for TMP loops.
template<uint8_t Index>
struct WrapperInitialize
{
    static void impl()
    {
        DollHouse::Buttons<Index>::initialize();
    }
};

template<uint8_t Index>
struct WrapperUpdate
{
    static void impl()
    {
        DollHouse::Buttons<Index>::update();
    }
};

template<uint8_t Index>
struct WrapperDeinitialize
{
    static void impl()
    {
        DollHouse::Buttons<Index>::deinitialize();
    }
};

// Wrappers for run-time loops.

bool buttonIsDoubleDownShortFinished(size_t const index);

bool buttonIsSingleDownShortFinished(size_t const index);

bool buttonIsDownLong(size_t const index);

} // namespace DollHouse

#endif // DOLL_HOUSE_BUTTONS_HPP
