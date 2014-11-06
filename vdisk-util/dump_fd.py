import os

for dir in os.listdir("/proc"):
    fddir = "/".join(["/proc", dir, "fd"])

    if os.path.isdir(fddir):
        print "~"
        os.system("ls -l {}".format("/".join(["/proc", dir, "exe"])))
        print ""
        os.system("ls -l {}".format(fddir))
