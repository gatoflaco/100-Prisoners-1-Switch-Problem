/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This file contains method definitions for prisoner.h.                                                   |
|===========================================================================================================|
*/

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include "prisoner.h"


/*============================================== Prisoner =================================================*/

/**
 * @brief CONSTRUCTOR - Initializes the object.
 * 
 * @param index unique number assigned to the prisoner for easier identification purposes.
 */
Prisoner::Prisoner(uint32_t index) : unique_index(index) {}

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
    std::cout << std::endl << "Prisoner #" << std::setfill('0') << std::setw(3) << this->unique_index
        << " declares that the challenge is complete!" << std::endl;
    *challenge_finished = true;
}


/*=============================================== Setter ==================================================*/

/**
 * @brief CONSTRUCTOR - Initializes the object.
 * 
 * @param index unique number assigned to the prisoner for easier identification purposes.
 */
Setter::Setter(uint32_t index) : Prisoner(index) {}

/**
 * @brief DECONSTRUCTOR - Frees memory.
 */
Setter::~Setter() {}

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
    while (!*challenge_finished) {
        sleep(WAIT_TIME);

        // try to enter the switch room
        switch_room->enter(this);
        if (*challenge_finished) {  // it's possible this changed while this prisoner was waiting to enter
            switch_room->exit(this);
            this->in_switch_room = false;
            continue;
        }
        this->in_switch_room = true;
        this->entered_count++;
        std::cout << std::endl << "Prisoner #" << std::setfill('0') << std::setw(3) << this->unique_index
            << " has entered the room." << std::endl;

        // if this prisoner has already flipped the switch up twice, they should just leave immediately
        if (switch_flip_count >= SETTER_MAX_COUNT) {
            std::cout << "  --> They leave without doing anything (because they are done)." << std::endl;
            switch_room->exit(this);
            this->in_switch_room = false;
            continue;
        }

        // check the state of the switch; if it's currently on, leave immediately
        switch_state current_state = switch_room->check_switch(this);
        if (current_state == on) {
            std::cout << "  --> They leave without doing anything (because the switch is on)." << std::endl;
            switch_room->exit(this);
            this->in_switch_room = false;
            continue;
        }

        // if the switch is currently off, turn it on
        if (current_state == off) {
            std::cout << "  --> They flip the switch to the on state." << std::endl;
            switch_room->flip_switch(this);
            this->switch_flip_count++;
        }

        // intentionally logically incorrect, for testing that the prison can verify incorrectness
        if (this->entered_count > 5) {
            this->declare_completion(challenge_finished);
        }//*/

        // leave the switch room so that the next prisoner may enter
        switch_room->exit(this);
        this->in_switch_room = false;
    }
}

/*============================================== Resetter =================================================*/

/**
 * @brief CONSTRUCTOR - Initializes the object.
 * 
 * @param index unique number assigned to the prisoner for easier identification purposes.
 */
Resetter::Resetter(uint32_t index) : Prisoner(index) {}

/**
 * @brief DECONSTRUCTOR - Frees memory.
 */
Resetter::~Resetter() {}

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
    while (!*challenge_finished) {
        sleep(WAIT_TIME);

        // try to enter the switch room
        switch_room->enter(this);
        if (*challenge_finished) {  // it's possible this changed while this prisoner was waiting to enter
            switch_room->exit(this);
            this->in_switch_room = false;
            continue;
        }
        this->in_switch_room = true;
        this->entered_count++;
        std::cout << std::endl << "Prisoner #" << std::setfill('0') << std::setw(3) << this->unique_index
            << " has entered the room." << std::endl;

        // if this prisoner has already flipped the switch down 198 times, they should just leave immediately
        if (switch_flip_count >= RESETTER_MAX_COUNT - (this->switch_starting_position == off ? 1 : 0)) {
            std::cout << "  --> They leave without doing anything (because they are done)." << std::endl;
            this->declare_completion(challenge_finished);
            switch_room->exit(this);
            this->in_switch_room = false;
            continue;
        }

        // check the state of the switch; if it's currently off, leave immediately
        switch_state current_state = switch_room->check_switch(this);
        if (current_state == off) {
            std::cout << "  --> They leave without doing anything (because the switch is off)." << std::endl;
            if (this->entered_count == 1) this->switch_starting_position == off;    // logically must be true
            switch_room->exit(this);
            this->in_switch_room = false;
            continue;
        }

        // if the switch is currently on, turn it off
        if (current_state == on) {
            std::cout << "  --> They flip the switch to the off state." << std::endl;
            switch_room->flip_switch(this);
            this->switch_flip_count++;
        }

        // can declare challenge complete if they just counted the final setter
        if (switch_flip_count >= RESETTER_MAX_COUNT - (this->switch_starting_position == off ? 1 : 0))
            this->declare_completion(challenge_finished);

        // leave the switch room so that the next prisoner may enter
        switch_room->exit(this);
        this->in_switch_room = false;
    }
}
