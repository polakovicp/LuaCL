import io
import os
import struct
import sys
from mbr import mbr
from ntfs import ntfs

def hexdump(chars, sep = ' ', width = 16):
    while chars:
        line = chars[:width]
        chars = chars[width:]
        line = line.ljust(width, '\000')
        print "%s%s%s" % (sep.join("%02x" % ord(c) for c in line), sep, quotechars(line))


def quotechars( chars ):
    return ''.join( ['.', c][c.isalnum()] for c in chars )

with open("/root/dc_file.txt", "rb") as f:

    partition = None
    ntfsReader = None

    while(True):
        sector = f.read(8)

        if not sector:
            break;

        sector = struct.unpack("Q", sector)[0]
        length = struct.unpack("Q", f.read(8))[0]
        data = f.read(length)

        #~ print("Sector: {:10} \tLength: {:10}".format(sector, length))
        #hexdump(data[:64], ' ', 16)

        if ntfsReader:
            # have reader
            if ntfsReader.contains_sector(sector):
                if ntfsReader.MftTable is None and ntfsReader.is_mft_start_sector(sector):
                    ntfsReader.init_mft(data)
                    #ntfsReader.MftTable.info()
                pass
            else:
                print("Sector: {:10} \tLength: {:10}".format(sector, length))
        elif partition and (sector == partition.StartLBA):
            with io.BytesIO(data) as bs:
                bootsec = ntfs.CBootsector()
                bs.readinto(bootsec)
                hexdump(data[:512])
                for field, type in bootsec._fields_:
                    if field is "Bpb":
                        for bpbfield, bpbtype in bootsec.Bpb._fields_:
                            print bpbfield, getattr(bootsec.Bpb, bpbfield)
                    else:
                        print field, getattr(bootsec, field)
            # have bootable partition - create NTFS reader
            ntfsReader = ntfs.NTFS(data)
            #ntfsReader.info()
        elif sector == 0 :
            #~ # parse MBR
            mbrp = mbr.MBRParser(data)
            if mbrp.get_value(mbrp.PartitionTable.Entry0.BootableFlag) == 0x80:
                partition = mbrp.PartitionTable.Entry0
            else:
                print("No bootable entry")
                break
        else:
            pass
            #print("Sector: {:10} \tLength: {:10}".format(sector, length))
