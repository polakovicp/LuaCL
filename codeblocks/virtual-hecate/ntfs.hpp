#ifndef NTFS_HPP
#define NTFS_HPP

#include <cstdint>

namespace Hecate
{
    typedef uint64_t MFT_REF;

    struct BIOS_PARAMETER_BLOCK
    {
        uint16_t BytesPerSector;
        uint8_t  SectorsPerCluster;
        uint16_t ReservedSectors;
        uint8_t  Fats;
        uint16_t RootEntries;
        uint16_t Sectors;
        uint8_t  MediaType;
        uint16_t SectorsPerFat;
        uint16_t SectorsPerTrack;
        uint16_t Heads;
        uint32_t HiddenSectors;
        uint32_t LargeSectors;
    } __attribute__((__packed__));

    struct NTFS_BOOT_SECTOR
    {
        uint8_t                 Jump[3];
        uint64_t                OemId;
        BIOS_PARAMETER_BLOCK    Bpb;
        uint8_t                 PhysicalDrive;
        uint8_t                 CurrentHead;
        uint8_t                 ExtendedBootSignature;
        uint8_t                 Reserved2;
        int64_t                 NumberOfSectors;
        int64_t                 MftLcn;
        int64_t                 MftMirrLcn;
        int8_t                  ClustersPerMftRecord;
        uint8_t                 Reserved0[3];
        int64_t                 ClustersPerIndexRecord;
        uint8_t                 Reserved1[3];
        uint64_t                VolumeSerialNumber;
        uint32_t                Checksum;
        uint8_t                 Bootstrap[426];
        uint16_t                Marker;
    } __attribute__((__packed__));

    enum NTFS_RECORD_TYPES
    {
        magic_FILE = 0x454c4946,
        magic_INDX = 0x58444e49,
        magic_HOLE = 0x454c4f48,
        magic_RSTR = 0x52545352,
        magic_RCRD = 0x44524352,
        magic_CHKD = 0x444b4843,
        magic_BAAD = 0x44414142,
        magic_empty = 0xffffffff
    };

    enum MFT_RECORD_FLAGS
    {
        MFT_RECORD_IN_USE		    = 0x0001,
        MFT_RECORD_IS_DIRECTORY	    = 0x0002,
        MFT_RECORD_IS_4			    = 0x0004,
        MFT_RECORD_IS_VIEW_INDEX	= 0x0008,
        MFT_REC_SPACE_FILLER		= 0xffff
    } __attribute__((__packed__));

    struct MFT_RECORD
    {
        NTFS_RECORD_TYPES   Magic;
        uint16_t            UsaOfs;
        uint16_t            UsaCount;
        uint64_t            Lsn;
    	uint16_t            Sequence;
        uint16_t            LinkCount;
        uint16_t            AttrOfs;
        MFT_RECORD_FLAGS    Flags;
        uint32_t            BytesInUse;
    	uint32_t            BytesAllocated;
        MFT_REF             BaseMftRecord;
        uint16_t            NextAttrInstance;
        uint16_t            Reserved;
        uint32_t            MftRecordNumber;
    } __attribute__((__packed__));

    enum ATTR_TYPES
    {
        AT_UNUSED			            = 0x00000000,
        AT_STANDARD_INFORMATION		    = 0x00000010,
        AT_ATTRIBUTE_LIST		        = 0x00000020,
        AT_FILE_NAME			        = 0x00000030,
        AT_OBJECT_ID			        = 0x00000040,
        AT_SECURITY_DESCRIPTOR		    = 0x00000050,
        AT_VOLUME_NAME			        = 0x00000060,
        AT_VOLUME_INFORMATION		    = 0x00000070,
        AT_DATA				            = 0x00000080,
        AT_INDEX_ROOT			        = 0x00000090,
        AT_INDEX_ALLOCATION		        = 0x000000a0,
        AT_BITMAP			            = 0x000000b0,
        AT_REPARSE_POINT		        = 0x000000c0,
        AT_EA_INFORMATION		        = 0x000000d0,
        AT_EA				            = 0x000000e0,
        AT_PROPERTY_SET			        = 0x000000f0,
        AT_LOGGED_UTILITY_STREAM	    = 0x00000100,
        AT_FIRST_USER_DEFINED_ATTRIBUTE	= 0x00001000,
        AT_END				            = 0xffffffff
    };

    enum ATTR_FLAGS
    {
        ATTR_IS_COMPRESSED      = 0x0001,
        ATTR_COMPRESSION_MASK	= 0x00ff,
        ATTR_IS_ENCRYPTED	    = 0x4000,
        ATTR_IS_SPARSE		    = 0x8000
    } __attribute__((__packed__));

    enum RESIDENT_ATTR_FLAGS
    {
        RESIDENT_ATTR_IS_INDEXED = 0x01
    } __attribute__((__packed__));

    typedef int64_t VCN;
    typedef int64_t LCN;

    struct ATTR_RECORD
    {
        ATTR_TYPES  Type;
        uint32_t    Length;
        uint8_t     NonResident;
        uint8_t     NameLength;
        uint16_t    NameOffset;
        ATTR_FLAGS  Flags;
        uint16_t    Instance;
        union
        {
            struct
            {
                uint32_t            ValueLength;
                uint16_t            ValueOffset;
                RESIDENT_ATTR_FLAGS ResidentFlags;
                int8_t              ReservedR;
            } __attribute__((__packed__));
            struct
            {
                VCN         LowestVcn;
                VCN         HighestVcn;
                uint16_t    MappingPairsOffset;
                uint8_t     CompressionUnit;
                uint8_t     Reserved1[5];
                int64_t     AllocatedSize;
                int64_t     DataSize;
                int64_t     InitializedSize;
                int64_t     CompressedSize;
            } __attribute__((__packed__));
        } __attribute__((__packed__));
    } __attribute__((__packed__));

    struct RUNLIST
    {
        LCN     Lcn;
        VCN     Vcn;
        int64_t Length;
    };
};

#endif
