include ../Rules.make

SOURCES       = main.cpp
OBJECTS       = main.o
EXECUTABLE = $(BIN_DIRECTORY)/11-bytes_reader

$(EXECUTABLE): $(OBJECTS) library
	@echo "----------------------------- Linking $(EXECUTABLE) -----------------------------"
	$(CXX) $(OBJECTS) $(LFLAGS) $(LIBS) -o $(EXECUTABLE)

library:
	cd $(SRC_DIRECTORY); make

clean:
	-find . -name "*.o" -exec rm -f {} \;
	rm -f $(EXECUTABLE)


