#!/usr/bin/env python

# ==========================================================================
# Author:         scps950707
# Email:          scps950707@gmail.com
# Created:        2018-06-19 15:20
# Last Modified:  2018-06-21 19:40
# Filename:       parse.py
# Description:    Plot the malloc/free sequence from paired file
# ==========================================================================
from __future__ import print_function
from sys import stdin
import matplotlib.pyplot as plt
from collections import Counter

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
                       'isMatched': False})


allocSizeSeq = [i['size'] for i in allSeq if i['op'] == "malloc"]
dic = dict()
dic = Counter(allocSizeSeq)
# for key in sorted(dic):
for key in dic:
    # print("%s: %s" % (key, dic[key]))
    plt.plot([key], [dic[key]], marker='o', markersize=6, color="red")
plt.title('Allocation Size Distribution', size=26)
plt.xlabel('Allocation Size (bytes)', size=26)
plt.ylabel('Number of allocations', size=26)
plt.xticks(fontsize=26)
plt.yticks(fontsize=26)
plt.xscale('log')
plt.yscale('log')
plt.show()
