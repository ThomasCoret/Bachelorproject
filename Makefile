COMPILE = g++ -std=c++14 -Wall -Wextra -g


OBJECTS = \
	libs/robot.o \
	libs/food.o \
	libs/world.o \
	main.o

HEADERS = \
	libs/food.h \
	libs/robot.h \
	libs/world.h

	
all:    	main

main:		$(OBJECTS)
		$(COMPILE) -o $@ $(OBJECTS)

libs/%.o:		%.cc $(HEADERS)
		$(COMPILE) -c $<

clean:
		rm -f main
		rm -f $(OBJECTS)
