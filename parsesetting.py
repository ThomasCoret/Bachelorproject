#!/bin/python

import numpy as np



f = open("graphs/socialreward/notseeotherrobots/100social50realfood.txt")

cols = 10
rows = 120

#create array
array = np.empty((cols,rows), dtype=object)
array2 = np.empty((cols,rows), dtype=object)

x = 0
y = 0

curnumber = ""

for line in f:
	linearray = [0]*rows
	begin = True
	first = True
	for char in line:
		if not begin:
			#number is finished
			if char == ';' or char == '&': 
				if first:
					array[y,x] = int(curnumber)
					first = False
				else:
					array2[y,x] = float(curnumber)
					first = True
					x+=1
				curnumber = ""
				
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

output = open("graphs/socialreward/seeotherrobots/100social50realavgfoodavg.txt","w+")

for x in range(rows):
	for y in range(cols):
		numsum += array2[y,x]
	output.write(str(x)+";"+str(round(numsum/cols,3))+"\n")
	numsum = 0
	
output = open("graphs/socialreward/seeotherrobots/100social50realmaxfoodavg.txt","w+")

for x in range(rows):
	for y in range(cols):
		numsum += array[y,x]
	output.write(str(x)+";"+str(round(numsum/cols,3))+"\n")
	numsum = 0
	
print("hello world")
