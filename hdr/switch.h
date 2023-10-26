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
        std::mutex lock;    // to prevent prisoners from entering the room when another is already inside
        Prisoner* current_occupant = nullptr;   // prisoner currently inside
    
    public:
        SwitchRoom(switch_state initial_state = off);
        ~SwitchRoom();

        void enter(Prisoner* prisoner);
        switch_state check_switch(Prisoner* prisoner);
        void flip_switch(Prisoner* prisoner);
        void exit(Prisoner* prisoner);
};


// class for the switch itself
class Switch
{
    private:
        switch_state state; // state of the switch; on or off

    public:
        Switch(switch_state initial_state = off);

        bool is_on();
        void flip();
};

#endif // SWITCH
