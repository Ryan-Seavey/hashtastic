#!/usr/bin/env python3
"""Toggle the bit at the specified offset.
Useful for creating test files for hashes.
Syntax: <cmdname> filename bit-offset"""

# the basis for this script was taken from:
# https://unix.stackexchange.com/questions/196251/change-only-one-bit-in-a-file

import sys
import os
import random

fname = sys.argv[1]
# get length of file
filelen = os.path.getsize(fname)
filebits = filelen * 8

# Open in read+write, binary mode; read 1 byte 
fp = open(fname, "r+b")

# Convert bit offset to bytes + leftover bits

# get bit position explicitly
target = int(sys.argv[2])

# printing is slow when processing many files
# print("Flipping the %s th bit in %s which is %d bytes long..." % (target, fname, filelen))

try:
    if target > filebits:
        print("There aren't that many bits in this (%d-byte) file!" % (filelen))
except IndexError:
    # pick a random bit position in the file
    target = random.randint(0, filebits-1)
    # print("Targeted bit is: ", target)

nbytes, nbits = divmod(target, 8)

fp.seek(nbytes, 0)
c = fp.read(1)

# Toggle bit at byte position `nbits`
toggled = bytes( [ ord(c)^(1<<nbits) ] ) 
# print(toggled) # diagnostic output

# Back up one byte, write out the modified byte
fp.seek(-1, 1)  # or absolute: fp.seek(nbytes, 0)
fp.write(toggled)
fp.close()
