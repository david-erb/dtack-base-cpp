TOP_DIRECTORY = ../..
SRC_DIRECTORY  = $(TOP_DIRECTORY)/src
LIB_DIRECTORY  = $(TOP_DIRECTORY)/lib
BIN_DIRECTORY  = $(TOP_DIRECTORY)/bin

CC            = gcc
CXX           = g++
DEFINES       = 
CFLAGS        = $(BITNESS) -fPIC -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = $(BITNESS) -fPIC -pipe -std=c++11 -Wall -g -Wno-unused-variable -W -D_REENTRANT $(DEFINES)
INCPATH       = -I. -I../include -I$(TOP_DIRECTORY)/include
LINK          = g++
LFLAGS        = $(BITNESS) -Wl,-rpath /usr/local/lib
LIBS          = \
    -L$(LIB_DIRECTORY) \
    -ldtack.base \
	-ldl \
    -lrt \
	-lpthread

AR            = ar cqs

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

LIBRARY = $(SRC_DIRECTORY)/library.a
