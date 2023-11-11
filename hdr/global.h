/* 100-Prisoners-1-Switch-Problem 
Isaac Jung

|===========================================================================================================|
|   This header contains a class used for holding global variables.                                         |
|===========================================================================================================|
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <mutex>
#include <unistd.h>

class Global;


// class for holding global variables and constants.
class Global
{
    public:
        static std::mutex output_mutex;                     // used for threaded console printing
        static inline int32_t PID = getpid();               // main thread's process ID
        const static inline uint8_t SETTER_MAX_COUNT = 2;   // count that setters want to set the switch
        const static inline float WAIT_TIME = 0.2;          // for avoiding repeated thread preemption
};

#endif // GLOBAL_H
