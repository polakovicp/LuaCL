#include "disk.hpp"
#include <algorithm>
#include <cstring>

using namespace std;

namespace Hecate
{
    Disk::Disk(uint8_t * data, uint64_t len)
    {
        Mbr = (MBR *)data;
    };

    PARTITION_ENTRY * Disk::GetBootablePartition()
    {
        for(int i = 0; i < 4; i++)
        {
            if(this->Mbr->PartitionTable.Entry[i].BootableFlag & 0x80)
            {
                return &Mbr->PartitionTable.Entry[i];
            }
        }

        return NULL;
    };
};
