/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This file contains method definitions for switch.h.                                                     |
|===========================================================================================================|
*/

#include "switch.h"


/*============================================= SwitchRoom ================================================*/

/**
 * @brief CONSTRUCTOR - Initializes the object.
 *
 * @param initial_state Initial state of the room's switch, on or off.
 */
SwitchRoom::SwitchRoom(switch_state initial_state)
{
    this->s = new Switch(initial_state);
}

/**
 * @brief DECONSTRUCTOR - Frees memory.
 */
SwitchRoom::~SwitchRoom()
{
    delete this->s;
}

/**
 * @brief OPENER - Interface for entering the room.
 *
 * @details This method is thread safe. All the prisoners may call it at any time, and only one will get in
 * at a time. However, a prisoner needs to be sure to use the exit() method when done, or everyone will get
 * stuck. While the prisoner is in the room, they may inspect the switch with check_switch() and they may
 * flip the switch with flip_switch(). These methods will not work for a prisoner who is not in the room.
 *
 * @param prisoner Prisoner trying to enter the room.
 */
void SwitchRoom::enter(Prisoner* prisoner)
{
    this->lock.lock();
    this->current_occupant = prisoner;
}

/**
 * @brief GETTER - Interface for getting the current switch state.
 *
 * @details This method will not return anything meaningful unless the prisoner is in the room. That means
 * that the enter() method must be called before this one.
 *
 * @param prisoner Prisoner trying to inspect the switch.
 * @return Returns a switch state - unknown when the prisoner is not allowed to know, otherwise on or off.
 */
switch_state SwitchRoom::check_switch(Prisoner* prisoner)
{
    if (prisoner != this->current_occupant) return unknown;
    return this->s->is_on() ? on : off;
}

/**
 * @brief TOGGLER - Interface for flipping the switch from off to on or vice versa.
 *
 * @details This method will do nothing if the prisoner is not currently in the room. That means that the
 * enter() method must be called before this one.
 *
 * @param prisoner Prisoner trying to flip the switch.
 */
void SwitchRoom::flip_switch(Prisoner* prisoner)
{
    if (prisoner != this->current_occupant) return;
    this->s->flip();
}

/**
 * @brief CLOSER - Interface for exiting the room.
 *
 * @details This method will do nothing if the prisoner is not currently in the room. This method needs to be
 * called once a prisoner is done being in the switch room, or no one else will be able to enter.
 *
 * @param prisoner Prisoner trying to exit the room.
 */
void SwitchRoom::exit(Prisoner* prisoner)
{
    if (prisoner != this->current_occupant) return;
    this->current_occupant = nullptr;
    this->lock.unlock();
}


/*=============================================== Switch ==================================================*/

/**
 * @brief CONSTRUCTOR - Initializes the object.
 *
 * @param initial_state Initial state of the switch, on or off. Note that it should never be unknown.
 */
Switch::Switch(switch_state initial_state)
{
    this->state = initial_state;
}

/**
 * @brief GETTER - Checks whether the switch is on.
 *
 * @return Returns true if the switch is on, false otherwise.
 */
bool Switch::is_on()
{
    return this->state == on;
}

/**
 * @brief TOGGLER - Flips the switch state from off to on or vice versa.
 */
void Switch::flip()
{
    this->state = (this->state == off) ? on : off;
}
