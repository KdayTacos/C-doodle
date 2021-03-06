CXX    = g++
CFLAGS = -Wall -g

# Windows builds 
ifeq ($(OS), Windows_NT)
	INCPATH += -IC:/Strawberry/c/include/
	LIBS = -lglut -lopengl32 -lglu32 -LC:/Strawberry/c/lib/

# Mac builds
else ifeq ($(shell uname), Darwin)
	LIBS = -framework GLUT -framework OpenGL -framework Cocoa

# Linux and all other builds
else
	LIBS = -lglut -lGL -lGLU
endif

all: sq1

clean:
	rm -f *.o sq1

depend:
	rm -f Makefile.bak
	mv Makefile Makefile.bak
	sed '/^# DEPENDENCIES/,$$d' Makefile.bak > Makefile
	echo '# DEPENDENCIES' >> Makefile
	$(CXX) -MM *.cpp >> Makefile

.c.o: 	
	$(CXX) $(CFLAGS) $(INCPATH) -c -o $@ $<

.cc.o: 	
	$(CXX) $(CFLAGS) $(INCPATH) -c -o $@ $<

.cpp.o: 	
	$(CXX) $(CFLAGS) $(INCPATH) -c -o $@ $<

sq1: main.o 
	$(CXX) $(CFLAGS) $(INCPATH) -o $@ $^ $(LIBPATH) $(LIBS)

# DEPENDENCIES
main.o: main.cpp
