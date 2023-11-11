/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This header contains the data structures for the switch room. The switch room consists of nothing more  |
| than a single switch. Therefore, there are two classes defined in this file, one for the SwitchRoom, one  |
| for the Switch, in which the SwitchRoom is in a "has a" relationship with the Switch (composition); that  |
| is, in this program, a Switch instance should not be instantiated outside of a SwitchRoom instance, and   |
| when the SwitchRoom is deleted, the Switch should be, too.                                                |
|===========================================================================================================|
*/

#ifndef SWITCH_H
#define SWITCH_H

#include <mutex>
#include "enums.h"

class Prisoner;     // comes from Prisoner.h, but must be forward declared to avoid circular includes

class SwitchRoom;
class Switch;


// class for the room containing the switch
class SwitchRoom
{
    private:
        Switch* s;                              // actual switch object
        std::mutex key;                         // prevents entering the room when already occupied
        Prisoner* current_occupant = nullptr;   // prisoner currently inside
        uint64_t entered_count = 0;             // total number of prisoners to enter the room
        uint32_t flipped_count = 0;             // total number of flips of the switch
    
    public:
        SwitchRoom(switch_state initial_state = switch_state::off);
        ~SwitchRoom();

        void unlock(Prisoner *prisoner);
        void enter(Prisoner* prisoner);
        switch_state check_switch(Prisoner* prisoner);
        void flip_switch(Prisoner* prisoner);
        void exit(Prisoner* prisoner, std::string description = "");
        void lock(Prisoner *prisoner);
        uint64_t get_entered_count();
        uint32_t get_flipped_count();
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

#endif // SWITCH_H
