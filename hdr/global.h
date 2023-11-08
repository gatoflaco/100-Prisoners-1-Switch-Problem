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


class Global
{
    public:
        static std::mutex output_mutex;       // used for threaded console printing
        static inline int32_t pid = getpid();   // main thread's process ID
};

#endif // GLOBAL_H
