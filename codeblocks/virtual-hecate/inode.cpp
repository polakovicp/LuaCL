#include "inode.hpp"
#include "disk.h"

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

        ProcessNonResidentAttribtues();
    };

    void Inode::ProcessNonResidentAttribtues()
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

                    }
                }
                break;
            case AT_END                             :
                break;
            }

            attr = (ATTR_RECORD *)((uint8_t *)attr + attr->Length);
        }
    };
};
