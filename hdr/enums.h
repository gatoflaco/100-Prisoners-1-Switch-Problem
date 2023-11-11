/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This header contains all of the enums specific to this program. This approach (as opposed to putting    |
| different enums in different header files) was taken to reduce the includes needed by other modules.      |
|===========================================================================================================|
*/

#ifndef ENUMS_H
#define ENUMS_H

/**
 * @brief What will take on the role of the warden.
 * 
 * @param os The operating system will decide; this is the default option which uses threads for prisoners.
 * @param pseudo The prisoners' order is decided by a random seed, but the program will be single threaded.
 * @param fixed A random permutation is decided in the beginning; the prisoners will follow that order.
 * @param seq Like fixed, but the order is specifically from 1 to 100.
 * @param fast Like sequential, but the resetter will enter any time the switch is in the on position.
 */
enum warden {
    os      = 0,
    pseudo  = 1,
    fixed   = 2,
    seq     = 3,
    fast    = 4
};

/**
 * @brief Version of strategy to use.
 * 
 * @param proper Only the resetter will declare completion, only when certain of guaranteed success.
 * @param improper The setters may declare completion too early, but may still succeed by luck.
 */
enum strategy {
    proper      = 0,
    improper    = 1,
};

/**
 * @brief Whether the switch is flipped on or off
 * 
 * @param unknown Special state that is only used when prisoners query the switch state when they shouldn't.
 * @param off Switch is flipped off; used to represent the RESET state that the resetter counts.
 * @param on Switch is flipped on; used to represent the SET state that the setters count.
 */
typedef enum {
    unknown = 0,
    off     = 1,
    on      = 2
} switch_state;

#endif // ENUMS_H
