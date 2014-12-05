#ifndef OPERATION_HPP
#define OPERATION_HPP

#ifdef __linux__
#elif _WIN32
    #include <windows.h>
#else

#endif

#include <cstdint>
#include <fstream>
#include <deque>

using namespace std;

namespace Hecate
{
    enum OP_TYPE
    {
        READ = 1,
        WRITE = 2
    };

    struct OPERATION
    {
        OP_TYPE     Type;
        uint64_t    Sector;
        uint64_t    Count;
        uint8_t *   Data;
    };

    typedef deque<OPERATION *> TaskQueue;

    typedef struct
    {
        TaskQueue *         Queue;
        CRITICAL_SECTION    CriticalSection;
    } TASK_CONTEXT;

//    class HecateException: public exception
//    {
//        virtual const char * what() const throw()
//        {
//            return "HecateException";
//        }
//    };

    static TASK_CONTEXT * InitializeTaskContext()
    {

    }

    int GetNextOperation(ifstream& ifs, OPERATION& op);
    void ReleaseOperationData(OPERATION& op);
};

#endif

