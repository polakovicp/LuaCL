import math
import struct

from ctypes import *

class NTFSException(Exception):
    def __init__(self, msg):
        Exception.__init__(self, msg)

class CBiosParameterBlock(Structure):
    _pack_ = 1
    _fields_ = [
        ("BytesPerSector"   , c_uint16),
        ("SectorsPerCluster", c_uint8 ),
        ("ReservedSectors"  , c_uint16),
        ("Fats"             , c_uint8 ),
        ("RootEntries"      , c_uint16),
        ("Sectors"          , c_uint16),
        ("MediaType"        , c_uint8 ),
        ("SectorsPerFat"    , c_uint16),
        ("SectorsPerTrack"  , c_uint16),
        ("Heads"            , c_uint16),
        ("HiddenSectors"    , c_uint32),
        ("LargeSectors"     , c_uint32)]

class CBootsector(Structure):
    _pack_ = 1
    _fields_ = [
        ("Jump"                 , c_uint8 * 3           ),
        ("OemId"                , c_uint64              ),
        ("Bpb"                  , CBiosParameterBlock   ),
        ("PhysicalDrive"        , c_uint8               ),
        ("CurrentHead"          , c_uint8               ),
        ("ExtBootSig"           , c_uint8               ),
        ("Reserved2"            , c_uint8               ),
        ("NumberOfSectors"      , c_int64               ),
        ("MftLcn"               , c_int64               ),
        ("MftMirrLcn"           , c_int64               ),
        ("ClustersPerMFTRec"    , c_int8                ),
        ("Reserved0"            , c_uint8 * 3           ),
        ("ClustersPerIndexRec"  , c_int8                ),
        ("Reserved1"            , c_uint8 * 3           ),
        ("VolumeSerialNo"       , c_uint64              ),
        ("Checksum"             , c_uint32              ),
        ("Bootstrap"            , c_int8 * 426          ),
        ("Marker"               , c_uint16              )]

class BiosParameterBlock:
    def __init__(self, data):
        self.BytesPerSector = struct.unpack("<H", data[:2])[0]
        self.SectorsPerCluster = struct.unpack("<B", data[2:3])[0]
        self.ReservedSectors = struct.unpack("<H", data[3:5])[0]
        self.Fats = struct.unpack("<B", data[5:6])[0]
        self.RootEntries = struct.unpack("<H", data[6:8])[0]
        self.Sectors = struct.unpack("<H", data[8:10])[0]
        self.MediaType = struct.unpack("<B", data[10:11])[0]
        self.SectorsPerFat = struct.unpack("<H", data[11:13])[0]
        self.SectorsPerTrack = struct.unpack("<H", data[13:15])[0]
        self.Heads = struct.unpack("<H", data[15:17])[0]
        self.HiddenSectors = struct.unpack("<I", data[17:21])[0]
        self.LargeSectors = struct.unpack("<I", data[21:25])[0]

class Bootsector:
    def __init__(self, data):
        self.Jump = data[:3]
        self.OemId = struct.unpack("<Q", data[3:11])[0]
        self.Bpb = BiosParameterBlock(data[11:])
        self.PhysicalDrive = struct.unpack("<B", data[36:37])[0]
        self.CurrentHead = struct.unpack("<B", data[37:38])[0]
        self.ExtBootSig = struct.unpack("<B", data[38:39])[0]
        self.Reserved2 = struct.unpack("<B", data[39:40])[0]
        self.NumberOfSectors = struct.unpack("<q", data[40:48])[0]
        self.MftLcn = struct.unpack("<q", data[48:56])[0]
        self.MftMirrLcn = struct.unpack("<q", data[56:64])[0]
        self.ClustersPerMFTRec = struct.unpack("<b", data[64:65])[0]
        self.Reserved0 = data[65:68]
        self.ClustersPerIndexRec = struct.unpack("<b", data[68:69])[0]
        self.Reserved1 = data[69:72]
        self.VolumeSerialNo = struct.unpack("<Q", data[72:80])[0]
        self.Checksum = struct.unpack("<I", data[80:84])[0]
        self.Bootstrap = data[84:510]
        self.Marker = struct.unpack("<H", data[510:512])[0]

class MftRecord:
    def __init__(self, data):
        """ Holds record basic information
        """
        self.Magic = struct.unpack("<cccc", data[:4])[0]
        self.USAOffset = struct.unpack("<H", data[4:6])[0]
        self.USACount = struct.unpack("<H", data[6:8])[0]
        self.Lsn = struct.unpack("<q", data[8:16])[0]
        self.SequenceNo = struct.unpack("<H", data[16:18])[0]
        self.LinkCount = struct.unpack("<H", data[18:20])[0]
        self.AttrOfs = struct.unpack("<H", data[20:22])[0]
        self.Flags = data[22:24]
        self.BytesInUse = struct.unpack("<I", data[24:28])[0]
        self.BytesAllocated = struct.unpack("<I", data[28:32])[0]
        self.BaseMRef = struct.unpack("<Q", data[32:40])[0]
        self.NextAttrInstance = struct.unpack("<H", data[40:42])[0]
        self.Reserved = struct.unpack("<H", data[42:44])[0]
        self.MRec = struct.unpack("<I", data[44:48])[0]

    def info(self):
        print("Magic {0}".format(self.Magic))
        print("USAOffset {0}".format(self.USAOffset))
        print("USACount {0}".format(self.USACount))
        print("Lsn {0}".format(self.Lsn))
        print("SequenceNo {0}".format(self.SequenceNo))
        print("LinkCount {0}".format(self.LinkCount))
        print("AttrOfs {0}".format(self.AttrOfs))
        #print("Flags {0}".format(self.Flags))
        print("BytesInUse {0}".format(self.BytesInUse))
        print("BytesAllocated {0}".format(self.BytesAllocated))
        print("BaseMRef {0}".format(self.BaseMRef))
        print("NextAttrInstance {0}".format(self.NextAttrInstance))
        print("MRec {0}".format(self.MRec))

class MftInode(MftRecord):
    def __init__(self, data):
        """ Has attribute streams
        """
        MftRecord.__init__(self, data)
        self.parse_attributes(data[self.AttrOfs:])

    def parse_attributes(self, data):
        pass

class MftTable(MftInode):
    def __init__(self, data):
        """ Holds another MFT records
        """
        MftInode.__init__(self, data)

class NTFS:
    def __init__(self, data):
        self.Bootsector = Bootsector(data)
        self.MftTable = None
        self.StartCluster = self.Bootsector.Bpb.HiddenSectors / self.Bootsector.Bpb.SectorsPerCluster
        self.EndCluster = math.ceil(
            (self.Bootsector.Bpb.HiddenSectors + self.Bootsector.NumberOfSectors) / self.Bootsector.Bpb.SectorsPerCluster)

    def init_mft(self, data):
        self.MftTable = MftTable(data)

    def info(self):
        print("Partition offset: {0} sectors".format(self.Bootsector.Bpb.HiddenSectors))
        print("MFT Lcn: {0} ({1} sectors)".format(self.Bootsector.MftLcn,
            self.Bootsector.MftLcn * self.Bootsector.Bpb.SectorsPerCluster))
        print("MFTMirror Lcn: {0} ({1} sectors)".format(self.Bootsector.MftMirrLcn,
            self.Bootsector.MftMirrLcn * self.Bootsector.Bpb.SectorsPerCluster))

    def is_mft_start_sector(self, abssector):
        return ((abssector - self.Bootsector.Bpb.HiddenSectors) == (self.Bootsector.MftLcn * self.Bootsector.Bpb.SectorsPerCluster))

    def contains_sector(self, abssector):
        return ((abssector >= (self.StartCluster * self.Bootsector.Bpb.SectorsPerCluster)) and
            (abssector <= (self.EndCluster * self.Bootsector.Bpb.SectorsPerCluster)))
