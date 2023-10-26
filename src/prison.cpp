/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This file contains method definitions for prison.h.                                                     |
|===========================================================================================================|
*/

#include <iostream>
#include <stdlib.h>
#include <thread>
#include "prison.h"

/**
 * @brief CONSTRUCTOR - Initializes the object.
 *
 * @param number_of_prisoners Number of prisoners to participate in the challenge.
 * @param seed Value used to seed rand.
 */
Prison::Prison(uint32_t number_of_prisoners, uint32_t seed)
{
    srand(seed);
    switch_state initial_state = (rand() % 2) == 0 ? on : off;
    
    for (uint32_t index = 1; index < number_of_prisoners; index++) {
        this->prisoners.push_back(new Setter(index));
    }
    this->prisoners.push_back(new Resetter(number_of_prisoners));

    this->switch_room = new SwitchRoom(initial_state);
}

/**
 * @brief DECONSTRUCTOR - Frees memory.
 */
Prison::~Prison()
{
    for (Prisoner* prisoner : this->prisoners) {
        delete prisoner;
    }

    delete this->switch_room;
}

/**
 * @brief SUB METHOD - This should be called by the main method.
 * 
 * The main purpose of this module is to carry out the 100-Prisoners-1-Switch-Problem simulation.
 * This method serves as the interface to start the simulation running.
 * This method starts threads for each prisoner, then immediately attempts to join them. They won't actually
 * join until one of the prisoners declares that the challenge is complete. At that point, this method will
 * check that all prisoners have actually in fact visited the room.
 * 
 * @return Returns true when the prisoners succeed at the challenge, false if they fail.
 */
bool Prison::challenge()
{
    bool challenge_finished = false;

    // start every prisoner on their task
    std::vector<std::thread> threads;
    for (Prisoner* prisoner : this->prisoners) {
        threads.push_back(
            std::thread(&Prisoner::perform_task, prisoner, &challenge_finished, this->switch_room)
        );
    }
    for (std::thread& thread : threads) {
        thread.join();  // only happens once a prisoner declares that the challenge is over
    }

    // check if all prisoners visited the room
    for (Prisoner* prisoner : this->prisoners) {
        if (!prisoner->has_been_in_switch_room()) {
            std::cout << "But the claim was wrong...." << std::endl;
            return false;
        }
    }
    std::cout << "The claim was correct." << std::endl;
    return true;
}