

# Assumes the following file exists in the proper place.
#include /home/cs425/code-egs/Makefile.defs
#The location for my makefile defs
include /mnt/c/Users/andyn/Desktop/School\ Files/Graphics/Makefile.defs 
# Next line says what options to use to compile programs
OPTIONS+=-Wall -std=c++14

OBJECTS = $(patsubst %.cc,%,$(wildcard *.cc))

all: scene

sphere.o: sphere.h sphere.cc common.o
	$(CC) sphere.cc -c $(OPTIONS)
cube.o: cube.h cube.cc common.o
	$(CC) cube.cc -c $(OPTIONS)
cube_door.o: cube_door.h cube_door.cc common.o
	$(CC) cube_door.cc -c $(OPTIONS)
common.o: common.cc common.h
	$(CC) common.cc -c $(OPTIONS)
flag.o: flag.cc common.o flag.h cube.o
	$(CC) flag.cc -c $(OPTIONS)
arrow.o: arrow.cc arrow.h common.h common.cc
	$(CC) arrow.cc -c $(OPTIONS)
box.o: box.cc box.h common.o
	$(CC) box.cc -c $(OPTIONS)
slope.o: slope.cc slope.h common.o
	$(CC) slope.cc -c $(OPTIONS)
scene: common.o common.h cube.o cube.h scene.cc sphere.h sphere.o flag.cc flag.h flag.o arrow.o box.o slope.o
	$(CC)  scene.cc common.o cube.o arrow.o box.o flag.o slope.o sphere.o $(InitShader) $(OPTIONS) $(LDLIBS) -o scene
clean:
	rm -f $(OBJECTS) *.o *~
