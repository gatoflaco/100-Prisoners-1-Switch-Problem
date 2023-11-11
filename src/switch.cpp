/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This file contains method definitions for switch.h.                                                     |
|===========================================================================================================|
*/

#include <iostream>
#include <thread>
#include "global.h"
#include "parser.h"
#include "prisoner.h"
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
 * @brief SEMAPHORE - Ensures thread safety.
 *
 * @details This method must be called by any prisoner before they can try to open the door.
 * Once they are done being in the room, they should be sure to call the lock() method to lock the door
 * behind them. If they don't no other prisoner will be allowed to enter the room.
 * This method is thread safe. When multiple threads try to call it at once, they will simply get stuck at
 * the door, and the OS will decide who gets in.
 *
 * @param prisoner Prisoner trying to unlock the room.
 */
void SwitchRoom::unlock(Prisoner* prisoner)
{
    Global::output_mutex.lock();
    if (Parser::debug_is_on())
        std::cout << "==" << std::this_thread::get_id() << "== In SwitchRoom::unlock()." << std::endl;
    if (Parser::verbose_is_on()) std::cout << std::endl << prisoner->to_string() <<
        " is trying to unlock the room." << std::endl;
    Global::output_mutex.unlock();
    this->key.lock();
    this->current_occupant = prisoner;
    if (Parser::verbose_is_on()) {
        Global::output_mutex.lock();
        std::cout << std::endl << prisoner->to_string() << " has unlocked the room.";
        Global::output_mutex.unlock();
    }
}

/**
 * @brief OPENER - Interface for entering the room.
 *
 * @details This method must be called after unlocking the room with unlock(), but before trying to do
 * anything with the switch. Once inside, a prisoner may inspect the switch with check_switch() and they may
 * flip the switch with flip_switch(). These methods will not work for a prisoner who is not in the room.
 *
 * @param prisoner Prisoner trying to enter the room.
 */
void SwitchRoom::enter(Prisoner* prisoner)
{
    if (prisoner != this->current_occupant) return;
    prisoner->set_in_switch_room(true);
    this->entered_count++;
    if (Parser::get_output_mode() != out_mode::silent) {
        Global::output_mutex.lock();
        std::cout << std::endl << prisoner->to_string() << " has entered the room." << std::endl;
        Global::output_mutex.unlock();
    }
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
    if (prisoner != this->current_occupant || !prisoner->is_in_switch_room()) return unknown;
    if (Parser::verbose_is_on() && Parser::get_output_mode() == out_mode::normal) {
        Global::output_mutex.lock();
        std::cout << "  --> They check the switch." << std::endl;
        Global::output_mutex.unlock();
    }
    return this->s->is_on() ? switch_state::on : switch_state::off;
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
    if (prisoner != this->current_occupant || !prisoner->is_in_switch_room()) return;
    this->s->flip();
    this->flipped_count++;
    if (Parser::get_output_mode() == out_mode::normal) {
        Global::output_mutex.lock();
        std::cout << "  --> They flip the switch to the " << (this->s->is_on() ? "on" : "off") <<
            " state." << std::endl;
        Global::output_mutex.unlock();
    }
}

/**
 * @brief CLOSER - Interface for exiting the room.
 *
 * @details This method will do nothing if the prisoner is not currently in the room. This method needs to be
 * called once a prisoner is done being in the switch room, or no one else will be able to enter. After this
 * method is executed successfully, the caller should then use lock() to lock the door behind them.
 *
 * @param prisoner Prisoner trying to exit the room.
 * @param description Info to print out regarding their visit.
 */
void SwitchRoom::exit(Prisoner* prisoner, std::string description)
{
    if (prisoner != this->current_occupant || !prisoner->is_in_switch_room()) return;
    if (Parser::get_output_mode() == out_mode::normal && !description.empty()) {
        Global::output_mutex.lock();
        std::cout << "  --> They " << description << "." << std::endl;
        Global::output_mutex.unlock();
    }
    prisoner->set_in_switch_room(false);
}

/**
 * @brief SEMAPHORE - Ensures thread safety.
 *
 * @details This method must be called by any prisoner after they have successfully closed the door.
 * This method serves as the final step in the process of accessing the switch room. If it is neglected, no
 * other prisoner will be able to enter the room, simply because they will get stuck trying to unlock the
 * room despite it still being unlocked.
 *
 * @param prisoner Prisoner trying to lock the room.
 */
void SwitchRoom::lock(Prisoner* prisoner)
{
    if (prisoner != this->current_occupant) return;
    if (Parser::verbose_is_on()) {
        Global::output_mutex.lock();
        std::cout << "  --> They lock the room behind them." << std::endl;
        Global::output_mutex.unlock();
    }
    this->current_occupant = nullptr;
    this->key.unlock();
}

/**
 * @brief GETTER - Interface for getting the total number of prisoners who have entered the room.
 *
 * @return Returns an integer greater than or equal to 0.
 */
uint64_t SwitchRoom::get_entered_count()
{
    return this->entered_count;
}

/**
 * @brief GETTER - Interface for getting the total number of times that the switch was flipped.
 *
 * @return Returns an integer greater than or equal to 0.
 */
uint32_t SwitchRoom::get_flipped_count()
{
    return this->flipped_count;
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
    return this->state == switch_state::on;
}

/**
 * @brief TOGGLER - Flips the switch state from off to on or vice versa.
 */
void Switch::flip()
{
    this->state = (this->state == switch_state::off) ? switch_state::on : switch_state::off;
}
