/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This file contains the entry point into the code.                                                       |
|===========================================================================================================|
*/

#include <iostream>
#include <sstream>
#include <stdint.h>
#include <random>
#include "prison.h"
#include "switch.h"


/**
 * @brief MAIN METHOD - Called when program is executed.
 * 
 * @param argc Number of arguments given by caller (including the token used to call).
 * @param argv Vector containing the arguments given by the caller.
 * @return Exit code representing the state of the program (0 means the program finished successfully).
 */
int32_t main(int32_t argc, char *argv[])
{
    uint32_t seed;
    if (argc == 2) {
        std::istringstream ss(argv[1]);
        ss >> seed;
    } else {
        std::random_device rd;
        std::mt19937 temp(rd());
        std::uniform_int_distribution<uint32_t> seed_distribution(0, UINT32_MAX);
        seed = seed_distribution(temp);
    }
    std::mt19937 g(seed);
    
    Prison::init_prisoners(100);

    std::uniform_int_distribution<uint8_t> distribution(0, 1);
    Prison::init_switch_room(distribution(g) == 0 ? switch_state::off : switch_state::on);
    
    bool success = Prison::challenge(g, false);
    Prison::free_memory();

    if (success) std::cout << "The prisoners all go free!" << std::endl;
    else std::cout << "The prisoners are doomed to die!" << std::endl;

    std::cout << std::endl << "The seed was: " << seed << std::endl;
    return 0;
}
