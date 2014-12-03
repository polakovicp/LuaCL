#ifndef INODE_HPP
#define INODE_HPP

#include "ntfs.hpp"
#include "range.hpp"

#include <cstdint>

namespace Hecate
{
    class Inode
    {
    public:
        MFT_RECORD * Record;
    private:
        void ProcessNonResidentAttribtuesRanges(class RangeLookupTable& table, ATTR_RECORD * attr);
    public:
        Inode(uint8_t * data, uint64_t len);
        void GetAttributesClustersRanges(RangeLookupTable& table);
    };
};

#endif // INODE_HPP
