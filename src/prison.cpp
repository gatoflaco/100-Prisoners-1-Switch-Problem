/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This file contains method definitions for prison.h.                                                     |
|===========================================================================================================|
*/

#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <thread>
#include "prison.h"


/**
 * @brief INITIALIZER - Initializes the prisoners.
 *
 * @param number_of_prisoners Number of prisoners to populate the prison.
 */
void Prison::init_prisoners(uint32_t number_of_prisoners)
{
    Prison::total_number_of_prisoners = number_of_prisoners;
    Prison::prisoner_unique_index_len = Prison::calculate_prisoner_unique_index_len(number_of_prisoners);

    // generate number_of_prisoners Prisoner objects
    for (uint32_t index = 1; index < number_of_prisoners; index++) {
        Prison::prisoners.push_back(new Setter(index));
    }
    Prison::prisoners.push_back(new Resetter(number_of_prisoners));
}

/**
 * @brief INITIALIZER - Initializes the switch room.
 *
 * @param initial_state Initial state of the prison's switch room's switch, on or off.
 */
void Prison::init_switch_room(switch_state initial_state)
{
    Prison::switch_room = new SwitchRoom(initial_state);
}

/**
 * @brief DELETER - Frees memory.
 */
void Prison::free_memory()
{
    for (Prisoner* prisoner : Prison::prisoners) {
        delete prisoner;
    }
    delete Prison::switch_room;
}

/**
 * @brief INTERNAL SETTER - Interface for setting the number of digits in a prisoner's unique index number.
 *
 * @param number_of_prisoners Number of prisoners to participate in the challenge.
 */
uint8_t Prison::calculate_prisoner_unique_index_len(uint32_t number_of_prisoners)
{
    uint8_t ret = 1;
    while (number_of_prisoners / 10 > 0) {
        number_of_prisoners /= 10;
        ret++;
    }
    return ret;
}

/**
 * @brief GETTER - Interface for getting the total number of prisoners in the prison.
 *
 * @return Returns the total number of prisoners, which should not change after prison is initialized.
 */
uint32_t Prison::num_prisoners()
{
    return Prison::total_number_of_prisoners;
}

/**
 * @brief GETTER - Interface for getting the number of digits in a prisoner's unique index number.
 *
 * @return Returns the prisoner unique index length, which should not change after prison is initialized.
 */
uint8_t Prison::prisoner_id_len()
{
    return Prison::prisoner_unique_index_len;
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
 * 
 * @param g Random engine, for example a std::mt19937 (Mersenne Twister engine).
 * @param threaded Whether the method should spawn prisoner threads or execute prisoner tasks one at a time.
 * @return Returns true when the prisoners succeed at the challenge, false if they fail.
 */
bool Prison::challenge(std::mt19937 g, bool threaded)
{
    if (
        !dynamic_cast<SwitchRoom*>(Prison::switch_room) ||
        Prison::prisoners.size() <= 2
    ) return false;    // can't challenge when switch_room is not instantiated or not enough prisoners

    bool challenge_finished = false;

    // shuffle for randomness
    std::shuffle(Prison::prisoners.begin(), Prison::prisoners.end(), g);

    // start every prisoner on their task
    if (threaded) {
        std::vector<std::thread> threads;
        for (Prisoner* prisoner : Prison::prisoners) {
            threads.push_back(
                std::thread(
                    &Prisoner::perform_task,
                    prisoner,
                    &challenge_finished,
                    Prison::switch_room,
                    true
                )
            );
        }
        for (std::thread& thread : threads) {
            thread.join();  // only happens once a prisoner declares that the challenge is over
        }
        
    } else {
        std::uniform_int_distribution<uint32_t> dist(0, Prison::total_number_of_prisoners - 1);
        while (!challenge_finished) {
            Prison::prisoners.at(dist(g))->perform_task(&challenge_finished, Prison::switch_room, false);
        }
    }

    // check if all prisoners visited the room
    for (Prisoner* prisoner : Prison::prisoners) {
        if (!prisoner->has_been_in_switch_room()) {
            std::cout << "But the claim was wrong...." << std::endl;
            return false;
        }
    }
    std::cout << "The claim was correct." << std::endl;
    return true;
}