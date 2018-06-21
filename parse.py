#!/usr/bin/env python

# ==========================================================================
# Author:         scps950707
# Email:          scps950707@gmail.com
# Created:        2018-06-18 17:25
# Last Modified:  2018-06-21 16:36
# Filename:       parse.py
# Description:    Find paired malloc/free sequence from raw data
# ==========================================================================
from __future__ import print_function
from sys import stdin

allSeq = []

# format example:
# malloc 5 0xfcfdc0
# free 5 0xfcfdc0

for line in stdin:
    data = line.rstrip('\n').split(' ')
    if data[0] == "malloc" or data[0] == "free":
        allSeq.append({'op': data[0],
                       'size': int(data[1]),
                       'addr': data[2],
                       'isPaired': False})

# print(allSeq)

# Get the malloc/free pairs
for i in range(0, len(allSeq)):
    if allSeq[i]['op'] == "malloc":
        for j in range(i+1, len(allSeq)):
            if allSeq[j]['isPaired']:
                continue
            else:
                if allSeq[j]['op'] == "malloc":
                    continue
                else:
                    if allSeq[j]['addr'] == allSeq[i]['addr']:
                        allSeq[i]['isPaired'] = True
                        allSeq[j]['isPaired'] = True
                        break


# Move Paired Sequence to new list
allSeqPaired = [i for i in allSeq if i['isPaired']]

# print("max alloc size:"+str(max([i['size'] for i in allSeqPaired])))
# print(allSeqPaired[i])
for i in range(0, len(allSeqPaired)):
    print("{} {} {}".format(allSeqPaired[i]['op'],
                            allSeqPaired[i]['size'],
                            allSeqPaired[i]['addr']))
