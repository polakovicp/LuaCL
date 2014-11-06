import os

#~ pid = 23782

#~ ldlib = "/proc/{}/maps".format(pid)

#~ with open(ldlib) as inf:
    #~ for line in inf.readlines():
        #~ pos = line.find('/')

        #~ if pos > -1:
            #~ module = line[pos:]
            #~ print module
            #~ os.system("nm -D {}".format(module))

import subprocess

args = [
    "strace",
    "/usr/lib/vmware/bin/vmware-vmx",
    "-ssnapshot.numRollingTiers=0",
    "-s", "vmx.stdio.keep=TRUE",
    "-#", "product=4;name=VMware Player;version=6.0.4;buildnumber=2249910;licensename=VMware Player;licenseversion=6.0;"
    "-@", "duplex=3;instance=0;msgs=ui",
    "/media/Data/Programs/Windows XP Professional/Windows XP Professional.vmx"
]

subprocess.Popen(" ".join(args))
