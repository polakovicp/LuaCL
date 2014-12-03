#include "partition.hpp"

namespace Hecate
{
    MftTable::MftTable(uint8_t * data, uint64_t len)
    {
        Inode * self = new Inode(data, len);
        Insert(self);
    };

    void MftTable::Insert(Inode * inode)
    {
        uint32_t index = inode->Record->MftRecordNumber;

        Table.insert(TableItem(index, inode));

        inode->GetAttributesClustersRanges(this->RangeTable);
    };

    void MftTable::Update(OPERATION& op)
    {
        /// 1) check if its from MFT range
        /// 1a) split by MFT record size
        /// 1b) process each extent
        /// 2) check if its data
        /// 2a) who own that data?

        this->RangeTable.Display();

        //op.Log->Debug("ede");
    };
};
