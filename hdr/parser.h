/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This header contains a class used for processing input. Should the input format change, this class can  |
| be updated accordingly. The way this class is used is to have the main code call the public parse()       |
| method. This will parse command line arguments separated by whitespace. The parse() method must be called |
| before the getter methods can be called. Other modules can also include this header and call the getters  |
| to inspect the program state based on the user-given arguments, but again, the parse() method must first  |
| be called, exactly once in the beginning of the program. Another thing this header file contains is a     |
| series of enums for very general program needs.                                                        |
|===========================================================================================================|
*/

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "enums.h"

class Parser;

/**
 * @brief Whether debug mode is set
 * 
 * @param d_off Debug mode is off, resulting in normal output.
 * @param d_on Debug mode is on, resulting in data structure info and extra flow control logging in output.
 */
enum debug_mode {
    d_off   = 0,
    d_on    = 1
};

/**
 * @brief Whether verbose mode is set.
 * 
 * @param v_off Verbose mode is off, resulting in less output.
 * @param v_on Verbose mode is on, resulting in more output.
 */
enum verb_mode {
    v_off   = 0,
    v_on    = 1
};

/**
 * @brief How much output should be printed.
 * 
 * @param normal Print out info about each prisoner who enters and what they do in the room.
 * @param halfway Only mention prisoners entering, not what they do.
 * @param silent Print out only the final conclusion.
 */
enum out_mode {
    normal  = 0,
    halfway = 1,
    silent  = 2
};


// parses command line input
class Parser
{
    private:
        static inline bool parse_called = false;            // prevents calling other methods before parse()
        static inline debug_mode d = debug_mode::d_off;         // debug mode, d_off by default
        static inline verb_mode v = verb_mode::v_off;           // verbose mode, v_off by default
        static inline out_mode o = out_mode::normal;            // output mode, normal by default
        static inline uint32_t num_p = 100;                     // number of prisoners, 100 by default
        static inline switch_state i_s = switch_state::unknown; // initial switch state, unknown by default
        static inline warden w = warden::os;                    // warden type, os by default
        static inline strategy strat = strategy::proper;        // strategy to use, "proper" by default
        static inline bool seed_provided = false;               // whether seed was given, false until found
        static inline uint32_t seed = 0;                        // user-given seed, empty string by default

        static void handle_option(const std::string& arg);
        static void handle_flags(const std::string& arg);
        static void handle_argument(const std::string& arg);

    public:
        static void parse(int32_t argc, char *argv[]);

        static bool debug_is_on();
        static bool verbose_is_on();
        static out_mode get_output_mode();
        static uint32_t get_number_of_prisoners();
        static switch_state get_initial_switch_state();
        static warden get_warden();
        static strategy get_strategy();
        static bool seed_is_from_user();
        static uint32_t get_seed();
};

#endif // PARSER_H
