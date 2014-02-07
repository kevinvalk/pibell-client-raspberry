# Platform detection
SYS := $(shell gcc -dumpmachine)
# For raspberry pi
ifneq (, $(findstring arm-linux, $(SYS)))
	CC=g++-4.7
	CFLAGS=-c -std=c++11 -DRASPBERRY
	LFLAGS=-lboost_system
# For cygwin
else ifneq (, $(findstring cygwin, $(SYS)))
	CC=g++
	CFLAGS=-c -Wall -Wno-switch -pedantic -std=c++11 -D__USE_W32_SOCKETS -D_WIN32_WINNT=0x0501 -DCYGWIN
	LFLAGS=-L/usr/local/lib -lws2_32 
endif

# Source settings
EXECUTABLE = pibell-client
SOURCES = main.cpp Client.cpp Config.cpp Audio.cpp CallAction.cpp
LFLAGS += -lboost_system -lboost_thread -lboost_chrono -lboost_filesystem -lao -ldl -lm

# Force always to rebuild main
 .PHONY: $(EXECUTABLE)

# System
OBJECTS=$(SOURCES:.cpp=.o)
all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $^ -o $@ $(LFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
