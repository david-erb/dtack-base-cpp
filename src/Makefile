include ../Rules.make
include Rules.make

OBJECTS       = \
	version.o \
	incidents/incident.o \
	incidents/incidents.o \
	environment.o \
	base.o \
	diwec_logging/diwec_logging.o \
	testing/testing.o \
	configuration/configuration.o \
	configuration/configuration_nlohmann.o \
	configuration/configuration_local.o \
	configuration/configuration_factory.o \
	limn/interface.o \
	limn/packets/limn.o \
	dispatcher/interface.o \
	string_token.o \
	string_walker.o \
	packet/packet.o \
	packet/load_json.o \
	packet/compose_json.o \
	cli/cli.o \
	bytes_reader/base.o \
	bytes_reader/binary_file.o \
	bytes_reader/socket.o \
	bytes_reader/factory.o
	
LIBRARY_NAME  = dtack.base
LIBRARY_FILE  = $(LIB_DIRECTORY)/lib$(LIBRARY_NAME).so

$(LIBRARY_FILE): $(OBJECTS)
	g++ $(OBJECTS) $(LIBFLAGS) -shared -O -Wl,-soname,lib$(LIBRARY_NAME).so -ldl -lpthread -o $(LIBRARY_FILE)

version:
	rm -f version.o

version.o: version.cpp \
		$(INC_DIRECTORY)/dtack.base.base.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

circular_buffer/circular_buffer.o: circular_buffer/circular_buffer.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.circular_buffer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

incidents/incident.o: incidents/incident.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.incidents.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

incidents/incidents.o: incidents/incidents.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.circular_buffer.h \
		$(INC_DIRECTORY)/dtack.base.incidents.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"
	
environment.o: environment.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

base.o: base.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"


diwec_logging/diwec_logging.o: diwec_logging/diwec_logging.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.diwec_logging.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

testing/testing.o: testing/testing.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.testing.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

limn/interface.o: limn/interface.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.limn.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

limn/packets/limn.o: limn/packets/limn.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.limn.h \
		limn/packets/limn.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

configuration/configuration.o: configuration/configuration.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.configuration.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

configuration/configuration_nlohmann.o: configuration/configuration_nlohmann.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.configuration.h \
		configuration/configuration_nlohmann.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

configuration/configuration_local.o: configuration/configuration_local.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.configuration.h \
		configuration/configuration_local.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

configuration/configuration_factory.o: configuration/configuration_factory.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.configuration.h \
		configuration/configuration_nlohmann.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

dispatcher/interface.o: dispatcher/interface.cpp \
		$(INC_DIRECTORY)/dtack.base.dispatcher.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

string_token.o: string_token.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.string_token.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

string_walker.o: string_walker.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.string_token.h \
		$(INC_DIRECTORY)/dtack.base.string_walker.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

packet/packet.o: packet/packet.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.string_token.h \
		$(INC_DIRECTORY)/dtack.base.string_walker.h \
		$(INC_DIRECTORY)/dtack.base.packet.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

packet/load_json.o: packet/load_json.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.string_token.h \
		$(INC_DIRECTORY)/dtack.base.string_walker.h \
		$(INC_DIRECTORY)/dtack.base.packet.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

packet/compose_json.o: packet/compose_json.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.string_token.h \
		$(INC_DIRECTORY)/dtack.base.string_walker.h \
		$(INC_DIRECTORY)/dtack.base.packet.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

cli/cli.o: cli/cli.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.configuration.h \
		$(INC_DIRECTORY)/dtack.base.cli.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

bytes_reader/base.o: bytes_reader/base.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.configuration.h \
		$(INC_DIRECTORY)/dtack.base.bytes_reader.h \
		bytes_reader/base.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

bytes_reader/binary_file.o: bytes_reader/binary_file.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.configuration.h \
		$(INC_DIRECTORY)/dtack.base.bytes_reader.h \
		bytes_reader/base.h \
		bytes_reader/binary_file.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

bytes_reader/socket.o: bytes_reader/socket.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.configuration.h \
		$(INC_DIRECTORY)/dtack.base.bytes_reader.h \
		bytes_reader/base.h \
		bytes_reader/socket.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

bytes_reader/factory.o: bytes_reader/factory.cpp \
		$(INC_DIRECTORY)/dtack.base.environment.h \
		$(INC_DIRECTORY)/dtack.base.base.h \
		$(INC_DIRECTORY)/dtack.base.configuration.h \
		$(INC_DIRECTORY)/dtack.base.bytes_reader.h \
		bytes_reader/base.h \
		bytes_reader/binary_file.h \
		bytes_reader/socket.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

clean:
	find . -name "*.o" -exec rm -f {} \;
	-rm -f $(LIBRARY_FILE)

