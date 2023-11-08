/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This file contains definitions for the static members in global.h that are non-constant integral types  |
| and therefore require proper construction outside the Global class declaration.                           |
|===========================================================================================================|
*/

#include "global.h"

std::mutex Global::output_mutex;
