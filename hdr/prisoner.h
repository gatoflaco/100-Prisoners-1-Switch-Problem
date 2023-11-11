/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This header contains the data structures for prisoners. There will be two roles that prisoners can take |
| on during the challenge; they will either be one of a group of "setters" who try to flip the switch to    |
| its "set" state (in this case, the on state), or they will be the single "resetter" who tries to flip the |
| switch to its "reset" state (in this case, the off state). Therefore, the design of this header file has  |
| a parent class and two child classes: the parent class is the abstract Prisoner class, which contains a   |
| lot of the data and method members shared in common between the two roles, while the Setter and Restter   |
| are child classes which inherit from Prisoner.                                                            |
|===========================================================================================================|
*/

#ifndef PRISONER_H
#define PRISONER_H

#include "global.h"
#include "switch.h"

class Prisoner;
class Setter;
class Resetter;


// base class for the two prisoner roles, setter and resetter
class Prisoner
{
    protected:
        const uint32_t unique_index;    // identity, for keeping track of who's who
        const std::string str_rep;      // memoized to_string_internal
        bool in_switch_room = false;    // true while this prisoner is in the switch room
        uint32_t flip_count = 0;        // how many times this prisoner has flipped the switch
        uint64_t entered_count = 0;     // how many times this prisoner has actually entered

        std::string to_string_internal(uint32_t index) const;

        void declare_completion(bool* challenge_finished);

    public:
        Prisoner(uint32_t index);
        virtual ~Prisoner() {};
        
        virtual std::string to_string() const = 0;
        bool is_in_switch_room();
        void set_in_switch_room(bool in_room);
        bool has_been_in_switch_room();

        virtual void perform_task(bool* challenge_finished, SwitchRoom* switch_room) = 0;
};


// child class of prisoner which will take on the role of flipping the switch on
class Setter : public Prisoner
{
    private:
        static const uint8_t target_count = Global::SETTER_MAX_COUNT;   // count setter wants to set
        const std::string str_rep;                              // memoized to_string_internal

        std::string to_string_internal(uint32_t index) const;

    public:
        Setter(uint32_t index);
        ~Setter();

        std::string to_string() const;

        void perform_task(bool* challenge_finished, SwitchRoom* switch_room);
};


// child class of prisoner which will take on the role of flipping the switch off
class Resetter : public Prisoner
{
    private:
        switch_state switch_start_state = unknown;  // can be updated to off in a specific case
        const uint64_t target_count;                // count resetter wants to reset
        const std::string str_rep;                  // memoized to_string_internal

        std::string to_string_internal(uint32_t index) const;
        uint64_t calculate_target_count() const;

    public:
        Resetter(uint32_t index);
        ~Resetter();

        std::string to_string() const;

        void perform_task(bool* challenge_finished, SwitchRoom* switch_room);
};

#endif // PRISONER_H
