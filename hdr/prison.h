/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This header contains the data structures for the prison. The role of the warden is to pick random       |
| prisoners at random times to go into the switch room. In practice, this describes the role of the OS, and |
| so there is no unique class or otherwise for the warden. However, the prison does consist of a bunch of   |
| prisoners, as well as t
|===========================================================================================================|
*/

#pragma once
#ifndef PRISON
#define PRISON

class Prison;

#include <time.h>
#include <stdint.h>
#include <vector>
#include "prisoner.h"
#include "switch.h"


// represents elements of the prison; the "warden" is just the OS that decides when prisoners get to go in
class Prison
{
    private:
        std::vector<Prisoner*> prisoners;
        SwitchRoom* switch_room;
    
    public:
        Prison(uint32_t number_of_prisoners = 100, uint32_t seed = time(nullptr));
        ~Prison();

        bool challenge();
};

#endif // PRISON