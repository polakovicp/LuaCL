#include "inode.hpp"
#include "disk.hpp"
#include "log.hpp"
#include <cstddef>
#include <algorithm>
#include <cstring>
#include <iostream>

using namespace std;

namespace Hecate
{
    Inode::Inode(uint8_t * data, uint64_t len)
    {
        Record = (MFT_RECORD *)data;

        // fixups
        uint16_t * usa = (uint16_t *)((uint8_t *)Record + Record->UsaOfs);
        for(uint16_t i = 1; i < Record->UsaCount; i++)
        {
            swap(usa[i], *(uint16_t*)&data[(SECTOR_SIZE * i) - 2]);

            if(usa[0] != *(uint16_t*)&data[(SECTOR_SIZE * i) - 2])
            {
                // TODO log
            }
        }
    };

    void Inode::GetAttributesClustersRanges(RangeLookupTable& table)
    {
        ATTR_RECORD * attr = (ATTR_RECORD *)((uint8_t *)this->Record + this->Record->AttrOfs);

        while((((uint8_t *)attr + offsetof(ATTR_RECORD, NonResident)) > (uint8_t *)this->Record) &&
            (((uint8_t *)attr + attr->Length) < ((uint8_t *)this->Record + this->Record->BytesInUse)))
        {
            switch(attr->Type)
            {
            case AT_UNUSED                          :
            case AT_STANDARD_INFORMATION            :
            case AT_ATTRIBUTE_LIST		            :
            case AT_FILE_NAME			            :
            case AT_OBJECT_ID			            :
            case AT_SECURITY_DESCRIPTOR		        :
            case AT_VOLUME_NAME			            :
            case AT_VOLUME_INFORMATION		        :
            case AT_DATA				            :
            case AT_INDEX_ROOT			            :
            case AT_INDEX_ALLOCATION		        :
            case AT_BITMAP			                :
            case AT_REPARSE_POINT		            :
            case AT_EA_INFORMATION		            :
            case AT_EA				                :
            case AT_PROPERTY_SET	                :
            case AT_LOGGED_UTILITY_STREAM           :
            case AT_FIRST_USER_DEFINED_ATTRIBUTE    :
                if(attr->NonResident)
                {
                    if(((uint8_t *)attr + attr->MappingPairsOffset) < ((uint8_t *)this->Record + this->Record->BytesInUse))
                    {
                        this->ProcessNonResidentAttribtuesRanges(table, attr);
                    }
                }
                break;
            case AT_END                             :
                return;
            }

            attr = (ATTR_RECORD *)((uint8_t *)attr + attr->Length);
        }
    };

    void Inode::ProcessNonResidentAttribtuesRanges(RangeLookupTable& table, ATTR_RECORD * attr)
    {
        VCN vcn = attr->LowestVcn;
        LCN lcn = 0;
        int64_t deltaxcn;
        const uint8_t *buf;
        const uint8_t *attr_end;
        uint8_t b;

        buf = (const uint8_t *)attr + attr->MappingPairsOffset;
        attr_end = (const uint8_t*)attr + attr->Length;

        while (buf < attr_end && *buf)
        {
            RANGE * range = new RANGE;

            b = *buf & 0xf;

            if (b)
            {
                for (deltaxcn = (int8_t)buf[b--]; b; b--)
                    deltaxcn = (deltaxcn << 8) + buf[b];
            }
            else
            {
                // TODO error
            }

            vcn = deltaxcn;

            if (!(*buf & 0xf0))
            {
                // TODO sparse
            }
            else
            {
                uint8_t b2 = *buf & 0xf;
                b = b2 + ((*buf >> 4) & 0xf);

                for (deltaxcn = (int8_t)buf[b--]; b > b2; b--)
                    deltaxcn = (deltaxcn << 8) + buf[b];

                lcn += deltaxcn;

                range->Begin = lcn;
                range->End = vcn + lcn;
            }

            table.Insert(this, attr, range);

            buf += (*buf & 0xf) + ((*buf >> 4) & 0xf) + 1;
        }
    };
};
