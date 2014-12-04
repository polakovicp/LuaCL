#include "operation.hpp"
#include "disk.hpp"

namespace Hecate
{
    int GetNextOperation(ifstream& ifs, OPERATION& op)
    {
        op.Type = READ;
        ifs.read((char *)&op.Sector, sizeof(uint64_t) * 2);

        if(ifs.gcount() != (sizeof(uint64_t) * 2))
        {
            if(ifs.eof() && (ifs.gcount() == 0))
                return 1;
            return 2;
        }

        op.Data = NULL;

        if(op.Count > 0)
        {
            op.Data = new uint8_t[op.Count];

            ifs.read((char *)op.Data, op.Count);

            if(ifs.gcount() != (int64_t)op.Count)
            {
                return 2;
            }
        }

        return 0;
    }

    void ReleaseOperationData(OPERATION& op)
    {
        if(op.Data)
            delete op.Data;
    }
};
