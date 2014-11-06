#include <iostream>
#include <vixDiskLib.h>

#define VIXDISKLIB_VERSION_MAJOR 5
#define VIXDISKLIB_VERSION_MINOR 5

#define VIXPLUGIN_DIRECTORY "/usr/lib/vmware-vix-disklib/lib64/"

struct VHK_DATA
{
    const char *            Disk;
    VixDiskLibConnection    Connection;
    VixDiskLibHandle        DiskHandle;
};

typedef VHK_DATA * PVHK_DATA;
