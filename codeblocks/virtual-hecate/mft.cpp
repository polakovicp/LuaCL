#include "partition.h"

namespace Hecate
{
    MftTable::MftTable(uint8_t * data, uint64_t len)
    {
        Inode * self = new Inode(data, len);

        Table.insert(TableItem(0, self));
    };

    void MftTable::Update(OPERATION& op)
    {
        /// 1) check if its from MFT range
        /// 1a) split by MFT record size
        /// 1b) process each extent
        /// 2) check if its data
        /// 2a) who own that data?


    };
};
