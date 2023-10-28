/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This header contains the data structures for the switch room.                                           |
|===========================================================================================================|
*/

#pragma once
#ifndef SWITCH
#define SWITCH

class SwitchRoom;
class Switch;

// typedef representing whether the switch is flipped on or off
// - unknown is only ever used when someone tries to inspect the state of the switch 
// - off represents the RESET state
// - on represents the SET state

/**
 * @brief Whether the switch is flipped on or off
 * 
 * @param unknown Special state that is only used when prisoners query the switch state when they shouldn't.
 * @param off Switch is flipped off; used to represent the RESET state that the resetter counts.
 * @param on Switch is flipped on; used to represent the SET state that the setters count.
 */
typedef enum {
    unknown = 0,
    off     = 1,
    on      = 2
} switch_state;

#include <mutex>
#include "prisoner.h"


// class for the room containing the switch
class SwitchRoom
{
    private:
        Switch* s;          // actual switch object
        std::mutex key;     // to prevent prisoners from entering the room when another is already inside
        Prisoner* current_occupant = nullptr;   // prisoner currently inside
    
    public:
        SwitchRoom(switch_state initial_state = switch_state::off);
        ~SwitchRoom();

        void unlock(Prisoner *prisoner);
        void enter(Prisoner* prisoner);
        switch_state check_switch(Prisoner* prisoner);
        void flip_switch(Prisoner* prisoner);
        void exit(Prisoner* prisoner, std::string description = "");
        void lock(Prisoner *prisoner);
};


// class for the switch itself
class Switch
{
    private:
        switch_state state; // state of the switch; on or off

    public:
        Switch(switch_state initial_state = switch_state::off);

        bool is_on();
        void flip();
};

#endif // SWITCH
