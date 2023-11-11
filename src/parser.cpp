/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This file contains method definitions for parser.h.                                                     |
|===========================================================================================================|
*/

#include <iostream>
#include <random>
#include "global.h"
#include "parser.h"

/**
 * @brief INITIALIZER - Initializes the parser.
 * 
 * @details By calling this function, the command line arguments will be parsed and become ready for reading.
 * 
 * @param argc Number of arguments given on command line when the program was called.
 * @param argv Vector containing the arguments themselves.
 */
void Parser::parse(int32_t argc, char *argv[])
{
    Parser::parse_called = true;

    int32_t itr = 1;
    bool multichar = false;

    while (itr < argc) {
        std::string arg(argv[itr]);         // cast to std::string

        if (multichar) {                                // options
            Parser::handle_option(arg);
            multichar = false;
        
        
        } else if (arg.at(0) == '-') {                  // flags
            if (arg.length() > 1 && arg.at(1) == '-') {
                multichar = true;
                continue;
            }
            Parser::handle_flags(arg);
            
        } else Parser::handle_argument(arg);            // arguments

        itr++;
    }

    if (!Parser::seed_provided) {
        std::random_device rd;
        std::mt19937 temp(rd());
        std::uniform_int_distribution<uint32_t> seed_distribution(0, UINT32_MAX);
        Parser::seed = seed_distribution(temp);
    }
}

/**
 * @brief HELPER - Handles the logic for command line arguments beginning with "--". These are options.
 * 
 * @param arg String containing a single command line argument.
 */
void Parser::handle_option(const std::string& arg)
{
    size_t pos = arg.find('=');
    if (pos == std::string::npos) {
        std::cout << "NOTE: option \'" << arg <<
            "\' missing =; ignored (all options in this program use =)" << std::endl;
        return;
    }
    std::string option = arg.substr(2, pos - 2);    // extract the option
    std::string value = arg.substr(pos + 1);        // extract the value
    if (option == "i" || option == "init" || option == "initial" || option == "initial_state") {
        if (value == "on" || value == "1" || value == "up" || value == "set") Parser::i_s = switch_state::on;
        else if (value == "off" || value == "0" || value == "down" || value == "reset")
            Parser::i_s = switch_state::off;
        else if (value != "unknown") std::cout << "NOTE: \'" << value <<
            "\' is not a valid initial switch state; ignored" << std::endl;
    } else if (option == "w" || option == "ward" || option == "warden") {
        if (value == "pseudo" || value == "pseudorand" || value == "pseudorandom" || value == "rand" ||
            value == "random") Parser::w = warden::pseudo;
        else if (value == "fixed") Parser::w = warden::fixed;
        else if (value == "seq" || value == "sequential") Parser::w = warden::seq;
        else if (value == "fast") Parser::w = warden::fast;
        else if (value != "os") std::cout << "NOTE: \'" << value <<
            "\' is not a valid warden type; ignored" << std::endl;
    } else if (option == "st" || option == "strat" || option == "strategy") {
        if (value == "i" || value == "improper") Parser::strat = strategy::improper;
        else if (value != "p" && value != "proper") std::cout << "NOTE: \'" << value <<
            "\' is not a valid strategy specifier; ignored" << std::endl;
    } else if (option == "se" || option == "seed") {
        try {
            Parser::seed = std::stoul(value);
            Parser::seed_provided = true;
        } catch (...) {
            std::cout << "NOTE: \'" << value << "\' isn\'t a valid seed value; ignored" << std::endl;
        }
    } else std::cout << "NOTE: \'" << option << "\' isn\'t a valid option; ignored" << std::endl;
}

/**
 * @brief HELPER - Handles the logic for command line arguments beginning with "-". These are flags.
 * 
 * @param arg String containing a single command line argument.
 */
void Parser::handle_flags(const std::string& arg)
{
    for (char c : arg.substr(1, arg.length() - 1)) {
        switch(c) {
            case 'd':
                Parser::d = debug_mode::d_on;
                break;
            case 'v':
                if (Parser::o == out_mode::silent)
                    std::cout << "NOTE: found -v after -s; ignored (silent overrides verbose)" << std::endl;
                else Parser::v = verb_mode::v_on;
                break;
            case 'h':
                Parser::o = out_mode::halfway;
                break;
            case 's':
                Parser::o = out_mode::silent;
                if (Parser::v == verb_mode::v_on)
                    std::cout << "NOTE: found -s after -v; silent mode forces verbose mode off" << std::endl;
                Parser::v = verb_mode::v_off;
                break;
            default:
                try {
                    uint64_t param = static_cast<uint64_t>(std::stoi(std::string(1, c)));
                    std::cout << "NOTE: bad flag \'" << param << "\'; ignored (looks like an int," <<
                        " did you mean to specify without a hyphen?)" << std::endl;
                } catch ( ... ) {
                    std::cout << "NOTE: bad flag \'" << c << "\'; ignored" << std::endl;
                }
                break;
        }
    }
}

/**
 * @brief HELPER - Handles the logic for any command line arguments not specified with "-" or "--".
 * 
 * @param arg String containing a single command line argument.
 */
void Parser::handle_argument(const std::string& arg)
{
    static uint32_t num_params = 0;
    try {   // see if it is an int
        uint32_t param = static_cast<uint32_t>(std::stoi(arg));
        if (num_params < 1) Parser::num_p = param;
        else std::cout << "NOTE: too many int arguments given; ignored <" << arg <<
            "> (the only argument in this program is for the number of prisoners)" << std::endl;
        num_params++;
    } catch ( ... ) {
        std::cout << "NOTE: couldn't parse command line argument <" << arg << ">; ignored" << std::endl;
    }
}

/**
 * @brief GETTER - Interface for getting whether the debug flag is set.
 *
 * @return Returns true when debug mode is enabled, false otherwise.
 * @throws std::logic_error When parse() hasn't been called first.
 */
bool Parser::debug_is_on()
{
    if (!Parser::parse_called) throw std::logic_error("Parser::parse() must be called first");

    return Parser::d == debug_mode::d_on;
}

/**
 * @brief GETTER - Interface for getting whether the verbose flag is set.
 *
 * @return Returns true when verbose mode is enabled, false otherwise.
 * @throws std::logic_error When parse() hasn't been called first.
 */
bool Parser::verbose_is_on()
{
    if (!Parser::parse_called) throw std::logic_error("Parser::parse() must be called first");

    return Parser::v == verb_mode::v_on;
}

/**
 * @brief GETTER - Interface for getting what output mode is set.
 *
 * @return Returns either out_mode::normal, out_mode::halfway, or out_mode::silent.
 * @throws std::logic_error When parse() hasn't been called first.
 */
out_mode Parser::get_output_mode()
{
    if (!Parser::parse_called) throw std::logic_error("Parser::parse() must be called first");

    return Parser::o;
}

/**
 * @brief GETTER - Interface for getting the number of prisoners specified on the command line.
 *
 * @return Returns an integer representing the number of prisoners to be used in the program.
 * @throws std::logic_error When parse() hasn't been called first.
 */
uint32_t Parser::get_number_of_prisoners()
{
    if (!Parser::parse_called) throw std::logic_error("Parser::parse() must be called first");

    return Parser::num_p;
}

/**
 * @brief GETTER - Interface for getting the initial switch state specified on the command line.
 *
 * @return Returns a switch_state as defined in switch.h.
 * @throws std::logic_error When parse() hasn't been called first.
 */
switch_state Parser::get_initial_switch_state()
{
    if (!Parser::parse_called) throw std::logic_error("Parser::parse() must be called first");

    return Parser::i_s;
}

/**
 * @brief GETTER - Interface for getting the warden type specified on the command line.
 *
 * @return Returns a warden as defined in prison.h.
 * @throws std::logic_error When parse() hasn't been called first.
 */
warden Parser::get_warden()
{
    if (!Parser::parse_called) throw std::logic_error("Parser::parse() must be called first");

    return Parser::w;
}


/**
 * @brief GETTER - Interface for getting the strategy to use specified on the command line.
 *
 * @return Returns a strategy as defined in prison.h.
 * @throws std::logic_error When parse() hasn't been called first.
 */
strategy Parser::get_strategy()
{
    if (!Parser::parse_called) throw std::logic_error("Parser::parse() must be called first");

    return Parser::strat;
}

/**
 * @brief GETTER - Interface for getting whether the seed was from the user or generated randomly.
 *
 * @return Returns true when seed is provided with the command line option "--seed=<value>", false otherwise.
 * @throws std::logic_error When parse() hasn't been called first.
 */
bool Parser::seed_is_from_user()
{
    if (!Parser::parse_called) throw std::logic_error("Parser::parse() must be called first");

    return Parser::seed_provided;
}

/**
 * @brief GETTER - Interface for getting the seed read from the command line or generated by the parser.
 *
 * @return Returns a uint32_t representing the seed to be used by any random behavior in the program.
 * @throws std::logic_error When parse() hasn't been called first.
 */
uint32_t Parser::get_seed()
{
    if (!Parser::parse_called) throw std::logic_error("Parser::parse() must be called first");

    return Parser::seed;
}

