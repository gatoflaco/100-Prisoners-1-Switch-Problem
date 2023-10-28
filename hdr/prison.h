/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This header contains the data structures for the prison. The role of the warden is to pick random       |
| prisoners at random times to go into the switch room. In practice, this describes the role of the OS, and |
| so there is no unique class or otherwise for the warden. However, the prison does consist of a bunch of   |
| prisoners, as well as the switch room. The class is static, as there should not be more than one instance |
| of the prison. The main program should initialize the prisoners with init_prisoners(), then it should     |
| initialize the switch room with init_switch_room(), and then it may call challenge() to begin the main    |
| loop. The free_memory() method should be called when the challenge() method is finished for a clean exit. |
|===========================================================================================================|
*/

#pragma once
#ifndef PRISON
#define PRISON

class Prison;

#include <stdint.h>
#include <vector>
#include <random>
#include "prisoner.h"
#include "switch.h"


// represents elements of the prison; the "warden" is just the OS that decides when prisoners get to go in
class Prison
{
    private:
        inline static uint32_t total_number_of_prisoners;  // number of prisoners in the prison
        inline static uint8_t prisoner_unique_index_len;   // char buffer size needed for prisoner ids
        inline static std::vector<Prisoner*> prisoners;    // actual prisoners in the prison
        inline static SwitchRoom* switch_room;             // room containing the switch

        static uint8_t calculate_prisoner_unique_index_len(uint32_t number_of_prisoners);
    
    public:
        static void init_prisoners(uint32_t number_of_prisoners);
        static void init_switch_room(switch_state initial_state);
        static void free_memory();

        static uint32_t num_prisoners();
        static uint8_t prisoner_id_len();

        static bool challenge(std::mt19937 g, bool threaded = true);
};

#endif // PRISON