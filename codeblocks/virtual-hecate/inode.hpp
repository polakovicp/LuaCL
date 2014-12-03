#include "ntfs.hpp"

#include <cstdint>

namespace Hecate
{
    class Inode
    {
    private:
        MFT_RECORD * Record;
    private:
        void ProcessNonResidentAttribtues();
    public:
        Inode(uint8_t * data, uint64_t len);
    };
};
