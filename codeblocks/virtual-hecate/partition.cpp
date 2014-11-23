#include "partition.h"
#include <algorithm>
#include <cstring>

using namespace std;

namespace Hecate
{
    Ntfs::Ntfs(uint8_t * boot, uint64_t len):
        Mft(NULL)
    {
        Bootsector = (NTFS_BOOT_SECTOR *)boot;

        StartSector = Bootsector->Bpb.HiddenSectors;
        EndSector = Bootsector->Bpb.HiddenSectors + Bootsector->NumberOfSectors;
    };

    void Ntfs::Update(OPERATION& op)
    {
        if(this->Mft)
        {

        }
        else if(this->IsMftStartSector(op.Sector))
        {
            this->Mft = new MftTable(op.Data, op.Count);
            this->Mft->Update(op);
        }
    };
};
