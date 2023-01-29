include ../../Rules.make
include ../Rules.make

SOURCES       = main.cpp
OBJECTS       = main.o
EXECUTABLE = $(BIN_DIRECTORY)/test_packets
LIBRARY = $(LIB_DIRECTORY)/libdtack.base.so

$(EXECUTABLE): $(OBJECTS) $(LIBRARY)
	$(CXX) $(OBJECTS) $(LFLAGS) $(LIBS) -o $(EXECUTABLE)

clean:
	-find . -name "*.o" -exec rm -f {} \;
	-rm -f $(EXECUTABLE)

