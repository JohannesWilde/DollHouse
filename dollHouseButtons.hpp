#ifndef DOLL_HOUSE_BUTTONS_HPP
#define DOLL_HOUSE_BUTTONS_HPP

#include "ArduinoDrivers/button.hpp"
#include "ArduinoDrivers/deprecated/buttonTimedMultiple.hpp"
#include "ArduinoDrivers/simplePinBit.hpp"

namespace DollHouse
{

constexpr SimplePin::State downState = SimplePin::State::Zero;
constexpr SimplePin::State upState = SimplePin::State::One;
constexpr TMP::ButtonTimedProperties::Duration_t durationShort = 2; // 50ms per cycle
constexpr TMP::ButtonTimedProperties::Duration_t durationLong = 8;
constexpr TMP::ButtonTimedProperties::Duration_t durationCombineMax = 6;
constexpr TMP::ButtonTimedProperties::Duration_t durationStateTimeout = 100;

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

typedef TMP::ButtonTimedMultiple<Button0, durationShort, durationLong, durationCombineMax> ButtonTimedMultiple0;
typedef TMP::ButtonTimedMultiple<Button1, durationShort, durationLong, durationCombineMax> ButtonTimedMultiple1;
typedef TMP::ButtonTimedMultiple<Button2, durationShort, durationLong, durationCombineMax> ButtonTimedMultiple2;
typedef TMP::ButtonTimedMultiple<Button3, durationShort, durationLong, durationCombineMax> ButtonTimedMultiple3;
typedef TMP::ButtonTimedMultiple<Button4, durationShort, durationLong, durationCombineMax> ButtonTimedMultiple4;
typedef TMP::ButtonTimedMultiple<Button5, durationShort, durationLong, durationCombineMax> ButtonTimedMultiple5;
typedef TMP::ButtonTimedMultiple<Button6, durationShort, durationLong, durationCombineMax> ButtonTimedMultiple6;
typedef TMP::ButtonTimedMultiple<Button7, durationShort, durationLong, durationCombineMax> ButtonTimedMultiple7;

template <uint8_t index>
class Buttons;
template <> class Buttons<0> : public ButtonTimedMultiple0 {/* intentionally empty */};
template <> class Buttons<1> : public ButtonTimedMultiple1 {/* intentionally empty */};
template <> class Buttons<2> : public ButtonTimedMultiple2 {/* intentionally empty */};
template <> class Buttons<3> : public ButtonTimedMultiple3 {/* intentionally empty */};
template <> class Buttons<4> : public ButtonTimedMultiple4 {/* intentionally empty */};
template <> class Buttons<5> : public ButtonTimedMultiple5 {/* intentionally empty */};
template <> class Buttons<6> : public ButtonTimedMultiple6 {/* intentionally empty */};
template <> class Buttons<7> : public ButtonTimedMultiple7 {/* intentionally empty */};


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
