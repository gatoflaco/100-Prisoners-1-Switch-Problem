/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This file contains the entry point into the code.                                                       |
|===========================================================================================================|
*/

#include <cstring>
#include <iostream>
#include "parser.h"
#include "prison.h"

static int32_t print_usage();
static void debug_print();

/**
 * @brief MAIN METHOD - Called when program is executed.
 * 
 * @param argc Number of arguments given by caller (including the token used to call).
 * @param argv Vector containing the arguments given by the caller.
 * @return Exit code representing the state of the program (0 means the program finished successfully).
 */
int32_t main(int32_t argc, char *argv[])
{
    if (argc > 1 && std::strcmp(argv[1], "--help") == 0) return print_usage();
    Parser::parse(argc, argv);
    int32_t pid = Parser::get_pid();
    bool debug_enabled = Parser::debug_is_on();
    if (debug_enabled) debug_print();
    
    Prison::init();
    bool success = Prison::challenge();
    Prison::free_memory();

    if (success) std::cout << std::endl << "The prisoners all go free!" << std::endl;
    else std::cout << std::endl << "The prisoners are doomed!" << std::endl;

    if (debug_enabled && !Parser::seed_is_from_user())
        std::cout << std::endl << "==" << pid << "== The seed was: " << Parser::get_seed() << std::endl;
    return 0;
}

/**
 * @brief HELPER - Prints info about the usage of the program.
 * 
 * @return Exit code representing the state of the program (0 means the program finished successfully).
 */
static int32_t print_usage()
{
    std::cout << "usage: ./simulation [flags] [options] [arguments]" << std::endl;
    std::cout << std::endl << "flags (single hyphens, can be combined):" << std::endl;
    std::cout << "\t-d : debug mode (prints extra state information)" << std::endl;
    std::cout << "\t\t--> for example, enable this to see what seed the program is using" << std::endl;
    std::cout << "\t-h : halfway mode (prints less output than normal)" << std::endl;
    std::cout << "\t\t--> output will state who enters, but not what decisions they make" << std::endl;
    std::cout << "\t-s : silent mode (prints minimal or no output, overrides -h and -v)" << std::endl;
    std::cout << "\t\t--> output will include only final result of challenge" << std::endl;
    std::cout << "\t-v : verbose mode (prints more output than normal)" << std::endl;
    std::cout << "\t\t--> for example, enable this to see prisoner entered counts" << std::endl;
    std::cout << std::endl << "options (double hyphens, cannot be combined):" << std::endl;
    std::cout << "\t--initial_state=<state> : initial state of the switch, random by default" << std::endl;
    std::cout << "\t\t--> equivalent options are --initial, --init, and --i" << std::endl;
    std::cout << "\t\t--> valid values of <state> are:"<< std::endl;
    std::cout << "\t\t\t1. unknown : switch starts in random position (default)" << std::endl;
    std::cout << "\t\t\t2. on/1/up : switch starts in on position" << std::endl;
    std::cout << "\t\t\t3. off/0/down : switch starts in off position" << std::endl;
    std::cout << "\t--warden=<type> : what plays the role of warden, OS by default" << std::endl;
    std::cout << "\t\t--> equivalent options are --ward and --w" << std::endl;
    std::cout << "\t\t--> valid values of <type> are:" << std::endl;
    std::cout << "\t\t\t1. os : threads are used; os decides who goes in (default)" << std::endl;
    std::cout << "\t\t\t2. pseudo/pseudorand/pseudorandom/rand/random : OS but single threaded" << std::endl;
    std::cout << "\t\t\t3. fixed : random permutation repeated until challenge ends" << std::endl;
    std::cout << "\t\t\t4. seq/sequential : like fixed but not a permutation; in order" << std::endl;
    std::cout << "\t\t\t5. fast : like seq but resetter goes in every other time" << std::endl;
    std::cout << "\t--strategy=<mode> : prisoner strategy, guaranteed success by default" << std::endl;
    std::cout << "\t\t--> equivalent options are --strat and --st" << std::endl;
    std::cout << "\t\t--> valid values of <mode> are:" << std::endl;
    std::cout << "\t\t\t1. proper : resetter declares finish, only when certain (default)" << std::endl;
    std::cout << "\t\t\t2. improper : setters can declare finish too early" << std::endl;
    std::cout << "\t--seed=<value> : seed to use, random by default" << std::endl;
    std::cout << "\t\t--> equivalent option is --se" << std::endl;
    std::cout << "\t\t--> <value> must be parsable as an unsigned 32-bit integer" << std::endl;
    std::cout << "\t\t\t-- if not provided, seed will be randomly generated at runtime" << std::endl;
    std::cout << "\t--help : print help message (what you are seeing here)" << std::endl;
    std::cout << std::endl << "arguments (no hyphens):" << std::endl;
    std::cout << "\tnumber of prisoners : total prisoners, must be >= 1, defaults to 100" << std::endl;
    std::cout << std::endl << "The order of flags, options, and arguments does not matter." << std::endl;
    std::cout << "For more details, please refer to the README." << std::endl;
    return 0;
}

/**
 * @brief HELPER - Prints out flag and option states when debug mode is enabled.
 */
static void debug_print() {
    int32_t pid = Parser::get_pid();
    bool verbose_enabled = Parser::verbose_is_on();
    out_mode o = Parser::get_output_mode();
    switch_state i_s = Parser::get_initial_switch_state();
    warden w = Parser::get_warden();
    strategy strat = Parser::get_strategy();
    uint32_t seed = Parser::get_seed();

    std::cout << "==" << pid << "== Debug mode is enabled. " <<
        "Look for lines preceeded by the PID." << std::endl;
    
    if (!verbose_enabled) std::cout << "==" << pid << "== Verbose mode: disabled" << std::endl;
    else std::cout << "==" << pid << "== Verbose mode: enabled" << std::endl;

    if (o == out_mode::normal) std::cout << "==" << pid << "== Output mode: normal" << std::endl;
    else if (o == out_mode::halfway) std::cout << "==" << pid << "== Output mode: halfway" << std::endl;
    else if (o == out_mode::silent) std::cout << "==" << pid << "== Output mode: silent" << std::endl;
    
    std::cout << "==" << pid << "== Number of prisoners: " << Parser::get_number_of_prisoners() << std::endl;

    if (i_s == switch_state::unknown) std::cout << "==" << pid << "== Switch state: unknown" << std::endl;
    else if (i_s == switch_state::on) std::cout << "==" << pid << "== Switch state: on" << std::endl;
    else if (i_s == switch_state::off) std::cout << "==" << pid << "== Switch state: off" << std::endl;
    else std::cout << "==" << pid << "== WARNING: SWITCH STATE APPEARS INVALID." << std::endl;

    if (w == warden::os) std::cout << "==" << pid << "== Warden: os" << std::endl;
    else if (w == warden::pseudo) std::cout << "==" << pid << "== Warden: pseudorandom" << std::endl;
    else if (w == warden::fixed) std::cout << "==" << pid << "== Warden: fixed" << std::endl;
    else if (w == warden::seq) std::cout << "==" << pid << "== Warden: sequential" << std::endl;
    else if (w == warden::fast) std::cout << "==" << pid << "== Warden: fast" << std::endl;
    else std::cout << "==" << pid << "== WARNING: WARDEN APPEARS INVALID." << std::endl;

    if (strat == strategy::proper) std::cout << "==" << pid << "== Strategy: proper" << std::endl;
    else if (strat == strategy::improper) std::cout << "==" << pid << "== Strategy: improper" << std::endl;
    else std::cout << "==" << pid << "== WARDNING: STRATEGY APPEARS INVALID." << std::endl;

    if (!Parser::seed_is_from_user()) std::cout << "==" << pid << "== Generated seed: " << seed << std::endl;
    else std::cout << "==" << pid << "== User-given seed: " << seed << std::endl;
}
