/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This header contains the data structures for the prison. The role of the warden is to pick random       |
| prisoners at random times to go into the switch room. In practice, this describes the role of the OS, and |
| so there is no unique class for the warden, only an enum (found in enums.h). However, the prison does     |
| consist of a bunch of prisoners, as well as the switch room. The class is static, as there should not be  |
| more than one instance of the prison. The main program should first initialize teverything with init(),   |
| and then it may call the challenge() method to begin the main loop. The free_memory() method should be    |
| called after challenge() for a clean exit.                                                                |
|===========================================================================================================|
*/

#ifndef PRISON_H
#define PRISON_H

#include <random>
#include <vector>
#include "prisoner.h"
#include "switch.h"

class Prison;


// represents elements of the prison
class Prison
{
    private:
        static inline bool init_called = false;             // prevents calling other methods before init()
        static inline uint32_t total_number_of_prisoners;   // number of prisoners in the prison
        static inline uint8_t prisoner_unique_index_len;    // char buffer size needed for prisoner ids
        static inline std::vector<Prisoner*> prisoners;     // actual prisoners in the prison
        static inline SwitchRoom* switch_room = nullptr;    // room containing the switch
        static inline std::mt19937* mt = nullptr;           // Mersenne Twister for pseudorandom behavior

        static uint8_t calculate_prisoner_unique_index_len(uint32_t number_of_prisoners);
    
    public:
        static void init();
        static void free_memory();

        static uint32_t num_prisoners();
        static uint8_t prisoner_id_len();

        static bool challenge();
};

#endif // PRISON_H
