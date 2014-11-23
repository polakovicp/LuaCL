#ifndef OPERATION_H
#define OPERATION_H

#include <cstdint>
#include <fstream>

using namespace std;

class HecateException: public exception
{
    virtual const char * what() const throw()
    {
        return "HecateException";
    }
};

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

int GetNextOperation(ifstream& ifs, OPERATION& op);
void ReleaseOperationData(OPERATION& op);

#endif
