#include "vmhook.h"

using namespace std;

VixError OpenDisk(PVHK_DATA);
VixError CloseDisk(PVHK_DATA);

int main()
{
    int ret = EXIT_SUCCESS;

    VHK_DATA data = {0};

    data.Disk = "/media/Data/Programs/Windows XP Professional/Windows XP Professional.vmdk";

    if(VIX_OK == OpenDisk(&data))
    {
        VixDiskLibInfo * vdInfo = NULL;

        VixError vErr = VixDiskLib_GetInfo(data.DiskHandle, &vdInfo);

        if(VIX_OK == vErr)
        {
            cerr << "Disk capacity " << vdInfo->capacity << endl;

            uint8 sector[VIXDISKLIB_SECTOR_SIZE];

            for(VixDiskLibSectorType cnt = 0; cnt < 20; cnt++)
            {
                vErr = VixDiskLib_Read(data.DiskHandle, cnt, 1, sector);

                if(VIX_OK != vErr)
                {
                    cerr << "Error: VixDiskLib_Read returned 0x" << hex << vErr << endl;
                    break;
                }
            }

            VixDiskLib_FreeInfo(vdInfo);
        }
        else
        {
            cerr << "Error: VixDiskLib_GetInfo returned 0x" << hex << vErr << endl;
        }
    }

    CloseDisk(&data);

    return ret;
}

VixError OpenDisk(PVHK_DATA hkd)
{
    VixError vErr = VixDiskLib_Init(VIXDISKLIB_VERSION_MAJOR, VIXDISKLIB_VERSION_MINOR,
        NULL, NULL, NULL, NULL);

    if(VIX_OK != vErr)
    {
        cerr << "Error: VixDiskLib_Init returned 0x" << hex << vErr << endl;
        return vErr;
    }

    VixDiskLibConnectParams cnxParams = {0};

    vErr = VixDiskLib_Connect(&cnxParams, &hkd->Connection);

    if(VIX_OK != vErr)
    {
        cerr << "Error: VixDiskLib_Connect returned 0x" << hex << vErr << endl;
        VixDiskLib_Exit();
        return vErr;
    }

    vErr = VixDiskLib_Open(hkd->Connection, hkd->Disk, VIXDISKLIB_FLAG_OPEN_SINGLE_LINK, &hkd->DiskHandle);

    if(VIX_OK != vErr)
    {
        cerr << "Error: VixDiskLib_Open returned 0x" << hex << vErr << endl;

        VixError e = VixDiskLib_Disconnect(hkd->Connection);

        if(VIX_OK != e)
        {
            cerr << "Error: VixDiskLib_Disconnect returned 0x" << hex << e << endl;
        }

        VixDiskLib_Exit();
    }

    return vErr;
}

VixError CloseDisk(PVHK_DATA hkd)
{
    VixError vErr;

    if(hkd->DiskHandle)
    {
        vErr = VixDiskLib_Close(hkd->DiskHandle);

        if(VIX_OK != vErr)
        {
            cerr << "Error: VixDiskLib_Close returned 0x" << hex << vErr << endl;
        }
    }

    if(hkd->Connection)
    {
        vErr = VixDiskLib_Disconnect(hkd->Connection);

        if(VIX_OK != vErr)
        {
            cerr << "Error: VixDiskLib_Disconnect returned 0x" << hex << vErr << endl;
        }
    }

    VixDiskLib_Exit();

    return vErr;
}
