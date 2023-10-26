/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This header contains the data structures for prisoners.                                                 |
|===========================================================================================================|
*/

#pragma once
#ifndef PRISONER
#define PRISONER

class Prisoner;
class Setter;
class Resetter;

#include <stdint.h>
#include "switch.h"

#define SETTER_MAX_COUNT    2
#define RESETTER_MAX_COUNT  198
#define WAIT_TIME           0.2


// base class for the two prisoner roles, setter and resetter
class Prisoner
{
    protected:
        const uint32_t unique_index;        // identity, for keeping track of who's who
        bool in_switch_room = false;        // true while this prisoner is in the switch room
        uint32_t switch_flip_count = 0;     // how many times this prisoner has flipped the switch
        uint64_t entered_count = 0;         // how many times this prisoner has actually entered

        void declare_completion(bool* challenge_finished);

    public:
        Prisoner(uint32_t index);
        virtual ~Prisoner() {};

        bool is_in_switch_room();
        bool has_been_in_switch_room();
        virtual void perform_task(bool* challenge_finished, SwitchRoom* switch_room) = 0;
};


// child class of prisoner which will take on the role of flipping the switch on
class Setter : public Prisoner
{
    public:
        Setter(uint32_t index);
        ~Setter();

        void perform_task(bool* challenge_finished, SwitchRoom* switch_room);
};


// child class of prisoner which will take on the role of flipping the switch off
class Resetter : public Prisoner
{
    private:
        switch_state switch_starting_position = unknown;    // can be updated to off in a specific case

    public:
        Resetter(uint32_t index);
        ~Resetter();

        void perform_task(bool* challenge_finished, SwitchRoom* switch_room);
};

#endif // PRISONER
