#!/usr/bin/env python

# ==========================================================================
# Author:         scps950707
# Email:          scps950707@gmail.com
# Created:        2018-06-19 15:21
# Last Modified:  2018-06-21 16:36
# Filename:       parse.py
# Description:    Create header contains array of malloc/free sequence
# ==========================================================================
from __future__ import print_function
from sys import stdin

allSeq = []

# input format example:
# malloc 5 0xfcfdc0
# free 5 0xfcfdc0

for line in stdin:
    data = line.rstrip('\n').split(' ')
    if data[0] == "malloc" or data[0] == "free":
        allSeq.append({'op': data[0],
                       'size': int(data[1]),
                       'addr': data[2],
                       'isMatched': False})


addrListMaxLen = 0

# We use an array of pointers to save retured address of malloc
# True:slot is used Fasle:slot is unused
addrList = []

# Get nonrepeat index
for i in range(0, len(allSeq)):
    if allSeq[i]['op'] == "malloc":
        try:
            storeIdx = addrList.index(False)
            addrList[storeIdx] = True
            allSeq[i]['idx'] = storeIdx
        except ValueError:
            addrList.append(True)
            allSeq[i]['idx'] = len(addrList) - 1
    else:
        # free neeed to search previous malloc's index
        for j in reversed(range(0, i)):
            if (allSeq[j]['op'] == "malloc"
                    and allSeq[j]['addr'] == allSeq[i]['addr']):
                rmIdx = allSeq[j]['idx']
                break
        allSeq[i]['idx'] = rmIdx
        addrList[rmIdx] = False
    addrListMaxLen = max(addrListMaxLen, len(addrList))


print("#define array_num {}".format(addrListMaxLen))
# In 64bit machine use long(8bytes) to save address
print("long allocate_array[array_num]={0};")


print("#define seq_num {}".format(len(allSeq)))

itemsPerLine = 10

print("// malloc:1 free:0")
print("int mem_ops[seq_num]= {")
for i in range(0, len(allSeq)):
    if i % itemsPerLine == 0 and i != 0:
        print("")
    endStr = '' if i == len(allSeq) - 1 else ','
    if allSeq[i]['op'] == 'malloc':
        print("1", end=endStr)
    else:
        print("0", end=endStr)
print("")
print("};")


print("// malloc:size free:0")
print("// max alloc unit:{}".format(max([i['size'] for i in allSeq])))
print("// total {} bytes allocated".format(sum([i['size'] for i in allSeq])))
print("int alloc_size[seq_num]= {")
for i in range(0, len(allSeq)):
    if i % itemsPerLine == 0 and i != 0:
        print("")
    endStr = '' if i == len(allSeq) - 1 else ','
    if allSeq[i]['op'] == 'malloc':
        print("%10d" % int(allSeq[i]['size']), end=endStr)
    else:
        print("%10d" % 0, end=endStr)
print("")
print("};")

print("// index in same pair must be same")
print("int alloc_idx[seq_num]={")
for i in range(0, len(allSeq)):
    if i % itemsPerLine == 0 and i != 0:
        print("")
    endStr = '' if i == len(allSeq) - 1 else ','
    print("%6d" % int(allSeq[i]['idx']), end=endStr)
print("")
print("};")
