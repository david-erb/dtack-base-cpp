TOP_DIRECTORY = ..
SRC_DIRECTORY  = $(TOP_DIRECTORY)/src
INC_DIRECTORY  = $(TOP_DIRECTORY)/include
OBJ_DIRECTORY  = $(TOP_DIRECTORY)/obj
LIB_DIRECTORY  = $(TOP_DIRECTORY)/lib
BIN_DIRECTORY  = $(TOP_DIRECTORY)/bin

CC            = gcc
CXX           = g++
DEFINES       = 
CFLAGS        = $(BITNESS) -fPIC -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = $(BITNESS) -fPIC -pipe -std=c++11 -Wall -g -Wno-unused-parameter -W -D_REENTRANT $(DEFINES)
INCPATH       = -I. -I../include -I$(TOP_DIRECTORY)/include
LIBFLAGS      = $(BITNESS)

AR            = ar cqs

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

LIBRARY = $(SRC_DIRECTORY)/library.a
