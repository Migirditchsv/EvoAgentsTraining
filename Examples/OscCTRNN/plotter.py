#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jan 29 15:23:28 2019

@author: Avalon
"""

#!/usr/bin/env python

# Simple visualizer for CTRNN Data

import matplotlib.pyplot as plt
import numpy as np
import csv

# Control Params:
filename = './data.txt'

n1 = []
n2 = []
t = []
counter = 0

with open(filename,'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        #print('ROW:',row,'| COUNTER:',counter)
        n1.append( float(row[0]) )
        n2.append( float(row[1]) )
        t.append(counter)
        counter+=1
plt.plot(t,n1,label='neuron 1')
plt.plot(t,n2,label='neuron 2')
plt.xlabel('time')
plt.ylabel('potential')
plt.title('neuron potential in time')
plt.legend()
plt.show()