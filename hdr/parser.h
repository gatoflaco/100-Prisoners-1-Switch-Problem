/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This header contains a class used for processing input. Should the input format change, this class can  |
| be updated accordingly. The way this class is used is to have the main code instantiate a single Parser   |
| object, whose constructor will parse command line arguments to determine options and states. This header  |
| also contains enumeration typedefs used by other modules.                                                 |
|===========================================================================================================|
*/

#pragma once
#ifndef PARSER
#define PARSER

#include <string>


/**
 * @brief Whether debug mode is set
 * 
 * @param d_off Debug mode is off, resulting in normal output.
 * @param d_on Debug mode is on, resulting in data structure info and extra flow control logging in output.
 */
typedef enum {
    d_off   = 0,
    d_on    = 1
} debug_mode;


/**
 * @brief Whether verbose mode is set.
 * 
 * @param v_off Verbose mode is off, resulting in less output.
 * @param v_on Verbose mode is on, resulting in more output.
 */
typedef enum {
    v_off   = 0,
    v_on    = 1
} verb_mode;


/**
 * @brief What will take on the role of the warden.
 * 
 * @param os The operating system will decide; this is the default option which uses threads for prisoners.
 * @param rand The prisoners' order is decided by a random seed, but the program will be single threaded.
 * @param fixed A random permutation is decided in the beginning; the prisoners will follow that order.
 * @param sequential Like fixed, but the order is specifically from 1 to 100.
 * @param fast Like sequential, but the resetter will enter any time the switch is in the on position.
 */
typedef enum {
    os          = 0,
    rand        = 1,
    fixed       = 2,
    sequential  = 3,
    fast        = 4
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


// parses command line input
class Parser
{
    private:
        debug_mode d = d_off;           // debug mode, d_off by default
        verb_mode v = v_off;            // verbose mode, v_off by default
        warden w = os;                  // warden, os by default
        strategy s = proper;            // strategy, proper by default
        uint32_t num_p = 100;           // number of prisoners to use in the challenge, 100 by default
        std::string i_s = "unknown";    // initial state of the switch, "unknown" by default
        std::string seed = "";          // user-given seed, empty string by default

    public:
        Parser(int32_t argc, char *argv[]); // constructor to read arguments and flags
        ~Parser();                          // deconstructor
};

#endif // PARSER
