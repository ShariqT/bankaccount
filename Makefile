CXX = g++
SOURCES = Security.cpp Customer.cpp main.cpp
CFLAGS = -g -Wall
PROGRAM = main
OBJS = main.o

all: build

build: $(SOURCES)
	$(CXX) $(CFLAGS) -o $(PROGRAM) $(SOURCES)

clean:
	rm -f $(PROGRAM)