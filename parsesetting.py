#!/bin/python

import numpy as np

print("hello world")

f = open("graphs/solosetting7random/solosetting1.20gen.txt")

cols = 100
rows = 20

#create array
array = np.empty((cols,rows), dtype=object)

x = 0
y = 0

curnumber = ""

for line in f:
	linearray = [0]*rows
	begin = True
	for char in line:
		if not begin:
			#number is finished
			if char == ';': 
				#print(str(x) + ";" + str(y) + ":" + curnumber)
				array[y,x] = float(curnumber)
				curnumber = ""
				x+=1
			else:
				if char != '\n':
					curnumber+=char
		#skip the first two characters
		if begin:
			if char == ':':
				begin = False
	y+=1
	x=0
	count = 0	

numsum = 0

output = open("graphs/solosetting7random/solosetting1.20genavg.txt","w+")

for x in range(rows):
	for y in range(cols):
		numsum += array[y,x]
	output.write(str(x)+";"+str(round(numsum/cols,3))+"\n")
	numsum = 0
			
