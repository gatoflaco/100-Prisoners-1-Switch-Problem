/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This file contains method definitions for prisoner.h.                                                   |
|===========================================================================================================|
*/

#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <unistd.h>
#include "parser.h"
#include "prisoner.h"
#include "prison.h"


/*============================================== Prisoner =================================================*/

/**
 * @brief CONSTRUCTOR - Initializes the object.
 * 
 * @param index unique number assigned to the prisoner for easier identification purposes.
 */
Prisoner::Prisoner(uint32_t index) : unique_index(index), str_rep(to_string_internal(index)) {}

/**
 * @brief INTERNAL SETTER - Interface for setting the string representation of the prisoner.
 *
 * @details This is a private method which should be called only in the member initializer list of the class
 * constructor, to set the const value of str_rep.
 *
 * @param index unique number assigned to the prisoner for easier identification purposes.
 * @return Returns a string of the form "Prisoner #x", where x is their index buffered with 0s.
 */
std::string Prisoner::to_string_internal(uint32_t index) const
{
    std::stringstream ret;
    ret << "Prisoner #" << std::setfill('0') << std::setw(Prison::prisoner_id_len()) << index;
    return ret.str();
}

/**
 * @brief GETTER - Interface for getting whether or not the prisoner is currently in the switch room.
 * 
 * @return Returns true when the prisoner is actively in the switch room, false otherwise.
 */
bool Prisoner::is_in_switch_room()
{
    return this->in_switch_room;
}

/**
 * @brief SETTER - Interface for setting whether or not the prisoner is currently in the switch room.
 * 
 * @param in_room Use true when setting the prisoner to be in the switch room, false otherwise.
 */
void Prisoner::set_in_switch_room(bool in_room)
{
    this->in_switch_room = in_room;
}

/**
 * @brief GETTER - Interface for getting whether or not the prisoner ever was in the switch room.
 * 
 * @return Returns true when the prisoner was in the switch room at least once, including currently,
 *  false otherwise.
 */
bool Prisoner::has_been_in_switch_room()
{
    return this->entered_count > 0;
}

/**
 * @brief HELPER - Prints message and sets the flag that tells everyone that the challenge is over.
 * 
 * @param challenge_finished Should be false when passed to this method, but will be set to true.
 */
void Prisoner::declare_completion(bool* challenge_finished)
{
    if (Parser::get_output_mode() != out_mode::silent)
        std::cout << std::endl << this->str_rep << " declares that the challenge is complete!" << std::endl;
    *challenge_finished = true;
}


/*=============================================== Setter ==================================================*/

/**
 * @brief CONSTRUCTOR - Initializes the object.
 * 
 * @param index unique number assigned to the prisoner for easier identification purposes.
 */
Setter::Setter(uint32_t index) : Prisoner(index), str_rep(to_string_internal(index)) {}

/**
 * @brief DECONSTRUCTOR - Frees memory.
 */
Setter::~Setter() {}

/**
 * @brief INTERNAL SETTER - Interface for setting the string representation of the setter.
 * 
 * @details This is a private method which should be called only in the member initializer list of the class
 * constructor, to set the const value of str_rep.
 *
 * @param index unique number assigned to the prisoner for easier identification purposes.
 * @return Returns a string of the form "Prisoner #x (Setter)", where x is their index buffered with 0s.
 */
std::string Setter::to_string_internal(uint32_t index) const
{
    return Prisoner::to_string_internal(index) + " (Setter)";
}

/**
 * @brief GETTER - Interface for getting the string representation of the setter.
 * 
 * @return Returns the str_rep built by to_string_internal() during object construction.
 */
std::string Setter::to_string() const
{
    return this->str_rep;
}

/**
 * @brief SUB METHOD - A thread should be started on this method.
 * 
 * The prison will have every prisoner attempt to carry out their tasks concurrently.
 * Setters want to enter the room and flip the switch to the on state if they can. They want to do this
 * exactly two times, and then never flip the switch again.
 * This method serves as the loop logic that a setter will follow to attempt to carry this out.
 * 
 * @param challenge_finished Starts false, and gets set to true when a prisoner thinks they have won.
 *  True breaks the loop, which would otherwise be infinite.
 * @param switch_room SwitchRoom object with which each prisoner interacts.
 */
void Setter::perform_task(bool* challenge_finished, SwitchRoom *switch_room)
{
    std::thread::id tid = std::this_thread::get_id();
    while (!*challenge_finished) {
        // try to unlock the switch room
        switch_room->unlock(this);
        if (*challenge_finished) {  // it's possible this changed while this prisoner was waiting to enter
            switch_room->lock(this);
            continue;
        }
        this->entered_count++;
        switch_room->enter(this);
        if (Parser::verbose_is_on()) std::cout << "  --> They have now entered " << this->entered_count <<
            " time(s)." << std::endl;

        // if this prisoner has already flipped the switch up twice, they should just leave immediately
        if (this->flip_count >= this->target_count) {
            switch_room->exit(this, "leave without doing anything (because they are done)");
        }

        // check the state of the switch; if it's currently on, leave immediately
        switch_state current_state = switch_room->check_switch(this);
        if (current_state == switch_state::on)
            switch_room->exit(this, "leave without doing anything (because the switch is on)");

        // if the switch is currently off, turn it on
        else if (current_state == switch_state::off) {
            switch_room->flip_switch(this);
            this->flip_count++;
            if (Parser::verbose_is_on()) std::cout << "  --> They have now flipped the switch " <<
                this->flip_count << " time(s)." << std::endl;
            switch_room->exit(this);
        }

        /* intentionally logically incorrect, for testing that the prison can verify incorrectness
        if (this->entered_count > 2 && this->flip_count >= this->target_count)
            this->declare_completion(challenge_finished);
        //*/

        // lock the switch room so that the next prisoner may unlock it
        switch_room->lock(this);
        if (Parser::get_warden() != warden::os) break;
        if (Parser::debug_is_on()) std::cout << "==" << tid << "== Sleeping for " << WAIT_TIME <<
            " second(s)." << std::endl;
        sleep(WAIT_TIME);
        if (Parser::debug_is_on()) std::cout << "==" << tid << "== Woke up." << std::endl;
    }
}

/*============================================== Resetter =================================================*/

/**
 * @brief CONSTRUCTOR - Initializes the object.
 * 
 * @param index unique number assigned to the prisoner for easier identification purposes.
 */
Resetter::Resetter(uint32_t index) : Prisoner(index), target_count(calculate_target_count()),
    str_rep(to_string_internal(index)) {}

/**
 * @brief DECONSTRUCTOR - Frees memory.
 */
Resetter::~Resetter() {}

/**
 * @brief INTERNAL SETTER - Interface for setting the string representation of the resetter.
 * 
 * @details This is a private method which should be called only in the member initializer list of the class
 * constructor, to set the const value of str_rep.
 *
 * @param index unique number assigned to the prisoner for easier identification purposes.
 * @return Returns a string of the form "Prisoner #x (Resetter)", where x is their index buffered with 0s.
 */
std::string Resetter::to_string_internal(uint32_t index) const
{
    return Prisoner::to_string_internal(index) + " (Resetter)";
}

/**
 * @brief GETTER - Interface for getting the string representation of the resetter.
 * 
 * @return Returns the str_rep built by to_string_internal() during object construction.
 */
std::string Resetter::to_string() const
{
    return this->str_rep;
}

/**
 * @brief INTERNAL SETTER - Interface for setting the target reset count for the resetter.
 * 
 * @details This is a private method which should be called only in the member initializer list of the class
 * constructor, to set the const value of target_count.
 *
 * @param index unique number assigned to the prisoner for easier identification purposes.
 * @return Returns a string of the form "Prisoner #x (Resetter)", where x is their index buffered with 0s.
 */
uint64_t Resetter::calculate_target_count() const
{
    return (Prison::num_prisoners() - 1) * SETTER_MAX_COUNT;
}

/**
 * @brief SUB METHOD - A thread should be started on this method.
 * 
 * The prison will have every prisoner attempt to carry out their tasks concurrently.
 * Resetters want to enter the room and flip the switch to the off state if they can. They want to do this
 * 198 times, except in a very special case. Once they do, they should declare the challenge finished.
 * This method serves as the loop logic that a resetter will follow to attempt to carry this out.
 * 
 * @param challenge_finished Starts false, and gets set to true when a prisoner thinks they have won.
 *  True breaks the loop, which would otherwise be infinite.
 * @param switch_room SwitchRoom object with which each prisoner interacts.
 */
void Resetter::perform_task(bool *challenge_finished, SwitchRoom *switch_room)
{
    std::thread::id tid = std::this_thread::get_id();
    while (!*challenge_finished) {
        // try to unlock the switch room
        switch_room->unlock(this);
        if (*challenge_finished) {  // it's possible this changed while this prisoner was waiting to enter
            switch_room->lock(this);
            continue;
        }
        this->entered_count++;
        switch_room->enter(this);
        if (Parser::verbose_is_on()) std::cout << "  --> They have now entered " << this->entered_count <<
            " time(s)." << std::endl;

        // check the state of the switch; if it's currently off, leave immediately
        switch_state current_state = switch_room->check_switch(this);
        if (current_state == switch_state::off) {
            if (this->entered_count == 1) this->switch_start_state = switch_state::off;
            switch_room->exit(this, "leave without doing anything (because the switch is off)");
            if (Parser::verbose_is_on()) std::cout << "  --> They note that they only have to count to " <<
                this->target_count - 1 << " now!" << std::endl;
        }

        // if the switch is currently on, turn it off
        else if (current_state == switch_state::on) {
            switch_room->flip_switch(this);
            this->flip_count++;
            if (Parser::verbose_is_on()) std::cout << "  --> They have now flipped the switch " <<
                this->flip_count << " time(s)." << std::endl;
            switch_room->exit(this);
        }

        // can declare challenge complete if they just counted the final setter
        if (this->flip_count >= this->target_count - (this->switch_start_state == switch_state::off ? 1 : 0))
            this->declare_completion(challenge_finished);

        // lock the switch room so that the next prisoner may unlock it
        switch_room->lock(this);
        if (Parser::get_warden() != warden::os) break;
        if (Parser::debug_is_on()) std::cout << "==" << tid << "== Sleeping for " << WAIT_TIME <<
            " second(s)." << std::endl;
        sleep(WAIT_TIME);
        if (Parser::debug_is_on()) std::cout << "==" << tid << "== Woke up." << std::endl;
    }
}
