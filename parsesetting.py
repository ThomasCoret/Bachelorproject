#!/bin/python

import numpy as np

print("hello world")

f = open("graphs/socialexperiment/setting75bestrobots.txt")

cols = 10
rows = 200

#create array
array = np.empty((cols,rows), dtype=object)

x = 0
y = 0

curnumber = ""

count = 0

for line in f:
	linearray = [0]*rows
	for char in line:
		#skip the first two characters
		if count > 1:
			#number is finished
			if char == ';': 
				#print(str(x) + ";" + str(y) + ":" + curnumber)
				array[y,x] = float(curnumber)
				curnumber = ""
				x+=1
			else:
				if char != '\n':
					curnumber+=char
		count+=1
	y+=1
	x=0
	count = 0	

numsum = 0

output = open("graphs/socialexperiment/setting75bestrobots.txt","w+")

for x in range(rows):
	for y in range(cols):
		numsum += array[y,x]
	output.write(str(x)+";"+str(round(numsum/10,3))+"\n")
	numsum = 0
			
