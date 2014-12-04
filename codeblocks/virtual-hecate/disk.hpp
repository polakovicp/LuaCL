#include <cstdint>

#define SECTOR_SIZE     512

namespace Hecate
{
    struct PARTITION_ENTRY
    {
        char        BootableFlag;
        uint8_t     StartCHS0;
        uint8_t     StartCHS1;
        uint8_t     StartCHS2;
        char        PartitionType;
        uint8_t     EndCHS0;
        uint8_t     EndCHS1;
        uint8_t     EndCHS2;
        uint32_t    StartLBA;
        int32_t     SizeInSectors;
    } __attribute__((__packed__));

    struct PARTITION_TABLE
    {
        uint8_t         DiskSignature[4];
        uint16_t        Unused;
        PARTITION_ENTRY Entry[4];
        uint16_t        Signature;
    };

    struct MBR
    {
        uint8_t             Code[440];
        PARTITION_TABLE     PartitionTable;
    };

    class Disk
    {
    public:
        MBR * Mbr;
    public:
        Disk(uint8_t * data, uint64_t len);
        PARTITION_ENTRY * GetBootablePartition();
    };
};
