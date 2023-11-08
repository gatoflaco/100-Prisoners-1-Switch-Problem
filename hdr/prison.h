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

#ifndef PRISON_H
#define PRISON_H

#include <random>
#include <vector>
#include "prisoner.h"
#include "switch.h"

class Prison;

/**
 * @brief What will take on the role of the warden.
 * 
 * @param os The operating system will decide; this is the default option which uses threads for prisoners.
 * @param pseudo The prisoners' order is decided by a random seed, but the program will be single threaded.
 * @param fixed A random permutation is decided in the beginning; the prisoners will follow that order.
 * @param seq Like fixed, but the order is specifically from 1 to 100.
 * @param fast Like sequential, but the resetter will enter any time the switch is in the on position.
 */
typedef enum {
    os      = 0,
    pseudo  = 1,
    fixed   = 2,
    seq     = 3,
    fast    = 4
} warden;

/**
 * @brief Version of strategy to use.
 * 
 * @param proper Only the resetter will declare completion, only when certain of guaranteed success.
 * @param improper The setters may declare completion too early, but may still succeed by luck.
 */
typedef enum {
    proper      = 0,
    improper    = 1,
} strategy;


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