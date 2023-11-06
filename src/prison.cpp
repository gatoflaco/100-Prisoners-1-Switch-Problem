/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This file contains method definitions for prison.h.                                                     |
|===========================================================================================================|
*/

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <thread>
#include "parser.h"
#include "prison.h"


/**
 * @brief INITIALIZER - Initializes the prison.
 *
 * @throws std::logic_error When number of prisoners < 1. There needs to be at least 1 resetter.
 */
void Prison::init()
{
    int32_t pid = Parser::get_pid();
    if (Parser::debug_is_on()) std::cout << std::endl << "==" << pid << "== In Prison::init()." << std::endl;
    Prison::init_called = true;

    uint32_t number_of_prisoners = Parser::get_number_of_prisoners();
    if (number_of_prisoners < 1) throw std::logic_error("Need at least 2 prisoners");
    Prison::mt = new std::mt19937(Parser::get_seed());

    Prison::total_number_of_prisoners = number_of_prisoners;
    Prison::prisoner_unique_index_len = Prison::calculate_prisoner_unique_index_len(number_of_prisoners);

    // generate number_of_prisoners Prisoner objects
    if (Parser::debug_is_on()) std::cout << "==" << pid << "== Creating " << number_of_prisoners <<
        " Prisoner objects (" << number_of_prisoners - 1 << " Setters, 1 Resetter)." << std::endl;
    for (uint32_t index = 1; index < number_of_prisoners; index++) {
        Prison::prisoners.push_back(new Setter(index));
    }
    Prison::prisoners.push_back(new Resetter(number_of_prisoners));

    // initialize switch room
    std::uniform_int_distribution<uint8_t> distribution(0, 1);
    switch_state initial_state = distribution(*Prison::mt) == 0 ? switch_state::off : switch_state::on;
    if (Parser::debug_is_on()) std::cout << "==" << pid << "== Creating switch room in initial position " <<
        (initial_state == switch_state::on ? "on." : "off.") << std::endl;
    Prison::switch_room = new SwitchRoom(initial_state);
}

/**
 * @brief DELETER - Frees memory.
 * @throws std::logic_error When init() hasn't been called first.
 */
void Prison::free_memory()
{
    if (!Prison::init_called) throw std::logic_error("Prison::init() must be called first");
    else if (Parser::debug_is_on())
        std::cout << std::endl << "==" << Parser::get_pid() << "== In Prison::free_memory()." << std::endl;

    for (Prisoner* prisoner : Prison::prisoners) {
        delete prisoner;
    }
    delete Prison::switch_room;
    delete Prison::mt;
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
 * @throws std::logic_error When init() hasn't been called first.
 */
uint32_t Prison::num_prisoners()
{
    if (!Prison::init_called) throw std::logic_error("Prison::init() must be called first");

    return Prison::total_number_of_prisoners;
}

/**
 * @brief GETTER - Interface for getting the number of digits in a prisoner's unique index number.
 *
 * @return Returns the prisoner unique index length, which should not change after prison is initialized.
 * @throws std::logic_error When init() hasn't been called first.
 */
uint8_t Prison::prisoner_id_len()
{
    if (!Prison::init_called) throw std::logic_error("Prison::init() must be called first");

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
 * @return Returns true when the prisoners succeed at the challenge, false if they fail.
 * @throws std::logic_error When init() hasn't been called first.
 */
bool Prison::challenge()
{
    if (!Prison::init_called) throw std::logic_error("Prison::init() must be called first");

    bool d = Parser::debug_is_on();
    warden w = Parser::get_warden();

    if (d) std::cout << "==" << Parser::get_pid() << "== In Prison::challenge()." << std::endl << std::endl;

    bool challenge_finished = false;

    // shuffle for randomness unless warden is seq or fast
    if (w < warden::seq) std::shuffle(Prison::prisoners.begin(), Prison::prisoners.end(), *Prison::mt);

    if (Parser::get_output_mode() != out_mode::silent)
        std::cout << "The challenge is commencing now!" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    // start every prisoner on their task
    if (w == warden::os) {
        std::vector<std::thread> threads;
        for (Prisoner* prisoner : Prison::prisoners) {
            threads.push_back(
                std::thread(
                    &Prisoner::perform_task,
                    prisoner,
                    &challenge_finished,
                    Prison::switch_room
                )
            );
        }
        for (std::thread& thread : threads) {
            thread.join();  // only happens once a prisoner declares that the challenge is over
        }
    } else if (w == warden::pseudo) {
        std::uniform_int_distribution<uint32_t> dist(0, Prison::total_number_of_prisoners - 1);
        while (!challenge_finished) {
            Prison::prisoners.at(dist(*Prison::mt))->perform_task(
                &challenge_finished,
                Prison::switch_room
            );
        }
    } else if (w == warden::fixed || w == warden::seq) {
        while (!challenge_finished) {
            for (uint32_t idx = 0; idx < Prison::total_number_of_prisoners; idx++) {
                Prison::prisoners.at(idx)->perform_task(
                    &challenge_finished,
                    Prison::switch_room
                );
                if (challenge_finished) break;
            }
        }
    } else {    // w == warden::fast
        while (!challenge_finished) {
            for (uint32_t idx = 0; idx < Prison::total_number_of_prisoners - 1; idx++) {
                Prison::prisoners.at(Prison::total_number_of_prisoners - 1)->perform_task(
                    &challenge_finished,
                    Prison::switch_room
                );
                if (challenge_finished) break;
                Prison::prisoners.at(idx)->perform_task(
                    &challenge_finished,
                    Prison::switch_room
                );
                if (challenge_finished) break;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    if (Parser::get_output_mode() != out_mode::silent)
        std::cout << std::endl << "The challenge ended in " << duration.count() << " seconds." << std::endl;

    // check if all prisoners visited the room
    for (Prisoner* prisoner : Prison::prisoners) {
        if (!prisoner->has_been_in_switch_room()) {
            if (Parser::get_output_mode() != out_mode::silent)
                std::cout << "But the claim was wrong...." << std::endl;
            return false;
        }
    }
    if (Parser::get_output_mode() != out_mode::silent) std::cout << "The claim was correct." << std::endl;
    return true;
}