#include "operation.h"
#include "ntfs.hpp"
#include "inode.hpp"

#include <cstdint>
#include <map>

namespace Hecate
{
    typedef map<uint32_t, Inode *> Inodes;
    typedef pair<uint32_t, Inode *> TableItem;

    class MftTable
    {
    private:
        Inodes  Table;
    public:
        MftTable(uint8_t * data, uint64_t len);
        void Update(OPERATION& op);
    };

    class Ntfs
    {
    private:
        MftTable * Mft;
    public:
        NTFS_BOOT_SECTOR * Bootsector;
        uint64_t StartSector;
        uint64_t EndSector;
    private:
        inline bool IsMftStartSector(uint64_t abssector)
        {
            return ((abssector - this->Bootsector->Bpb.HiddenSectors) == (uint64_t)(this->Bootsector->MftLcn * this->Bootsector->Bpb.SectorsPerCluster));
        };
    public:
        Ntfs(uint8_t * boot, uint64_t len);
        inline bool ContainsSector(uint64_t abssector)
        {
            return ((abssector >= this->StartSector) && (abssector <= this->EndSector));
        };
        void Update(OPERATION& op);
    };
};
