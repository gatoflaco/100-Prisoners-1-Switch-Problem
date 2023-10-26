/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This file contains the entry point into the code.                                                       |
|===========================================================================================================|
*/

#include <iostream>
#include <stdint.h>
#include "prison.h"


/**
 * @brief MAIN METHOD - Called when program is executed.
 * 
 * @param argc Number of arguments given by caller (including the token used to call).
 * @param argv Vector containing the arguments given by the caller.
 * @return Exit code representing the state of the program (0 means the program finished successfully).
 */
int32_t main(int32_t argc, char *argv[])
{
    Prison prison;
    
    bool success = prison.challenge();
    if (success) std::cout << "The prisoners all go free!" << std::endl;
    else std::cout << "The prisoners are doomed till they die!" << std::endl;

    return 0;
}
