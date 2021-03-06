#include "hecate.hpp"
#include "operation.hpp"
#include "disk.hpp"
#include "partition.hpp"

using namespace std;
using namespace Hecate;

int main()
{
    const char * log = "/root/projects/dc-file.txt";

    //ReadEvents(log);

    //Logging::Log f;

    Logging::LOG << "meh";

    class VHecate worker;

    return 0;
}

void ReadEvents(const char * log)
{
    ifstream ifs;

    ifs.open(log, ios::binary);

    if(ifs.is_open())
    {
        OPERATION   op;
        Disk *      disk = NULL;
        PARTITION_ENTRY * pentry = NULL;
        Ntfs *      ntfs = NULL;

        while(!GetNextOperation(ifs, op))
        {
            cout << "Sector " << op.Sector << " Length " << op.Count << endl;

            /// 1) wait for MBR sector
            /// 2) get active NTFS partition LBA from MBR
            /// 3) wait for LBA address
            /// 4) get NTFS bootsector and MFT LCN
            /// 5) wait for MFT LCN
            /// 6) parse $MFT

            if(ntfs)
            {
                if(ntfs->ContainsSector(op.Sector))
                {
                    ntfs->Update(op);
                    continue;
                }
                else
                {
                }
            }
            else if(pentry && (pentry->StartLBA == op.Sector))
            {
                ntfs = new Ntfs(op.Data, op.Count);
                continue;
            }
            else if(disk == NULL)
            {
                if(0 == op.Sector)
                {
                    disk = new Disk(op.Data, op.Count);
                    pentry = disk->GetBootablePartition();
                    continue;
                }
            }

            Hecate::ReleaseOperationData(op);
        }

        ifs.close();
    }
}
