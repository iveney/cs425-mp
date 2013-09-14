# if use g++, use -std=c++0x to enable c++0x
# if use clang, use -std=c++11 to enable c++11
CC=clang++
CXXFLAGS=-Wall
ifeq "$(CC)" "g++"
	# only use c++0x feature when it is >= 4.3
	GCCVERSIONREQ := $(shell expr `g++ -dumpversion | cut -f1 -d.` \>= 4.3)
	ifeq "$(GCCVERSIONREQ)" "1"
		CXXFLAGS += -std=c++0x
	endif
else ifeq "$(CC)" "clang++"
	LLVMVERSIONREQ := $(shell expr `clang++ -v 2>&1 | grep "version" | cut -d' ' -f 4` \>= 4.2)
	ifeq "$(LLVMVERSIONREQ)" "1"
		CXXFLAGS += -std=c++11
	endif
endif

# object dir
ODIR=obj

# shared source files
SRC=message.cpp

SERVER_SRC=$(SRC)
SERVER_SRC+=server_main.cpp server.cpp tcp_connection.cpp
SERVER_HDR=$(SERVER_SRC:.cpp=.h)
_SERVER_OBJ=$(SERVER_SRC:.cpp=.o) 
SERVER_OBJ=$(_SERVER_OBJ:%=$(ODIR)/%)
SERVER_OBJ_G=$(SERVER_OBJ:.o=.o-g)
SERVER=dgrepd

CLIENT_SRC=$(SRC)
CLIENT_SRC+=client_main.cpp client.cpp name_parser.cpp
CLIENT_HDR=$(CLIENT_SRC:.cpp=.h)
_CLIENT_OBJ=$(CLIENT_SRC:.cpp=.o) 
CLIENT_OBJ=$(_CLIENT_OBJ:%=$(ODIR)/%)
CLIENT_OBJ_G=$(CLIENT_OBJ:.o=.o-g)
CLIENT=dgrep

GTEST=gtest-1.6.0
GTEST_INCLUDE=$(GTEST)/include
UNITTEST_SRC=$(SRC)
UNITTEST_SRC+=unittest.cpp
UNITTEST_HDR=$(UNITTEST_SRC:.cpp=.h)
_UNITTEST_OBJ=$(UNITTEST_SRC:.cpp=.o)
UNITTEST_OBJ=$(_UNITTEST_OBJ:%=$(ODIR)/%)
UNITTEST_OBJ_G=$(UNITTEST_OBJ:.o=.o-g)
UNITTEST=unittest

CFLAGS=
CPPFLAGS=
LDFLAGS=-lboost_system-mt -lboost_program_options-mt -lpthread

BINS=$(SERVER) $(CLIENT)
DBGS=$(BINS:%=%-g)

all:dir opt dbg

dir:
	@mkdir -p obj

opt: CXXFLAGS += -O3
opt: $(BINS)

dbg: CXXFLAGS += -g -DDEBUG -O3
dbg: $(DBGS)

dgrepd:$(SERVER_OBJ)
	$(CC) $(LDFLAGS) -o $(SERVER) $(SERVER_OBJ)

dgrepd-g:$(SERVER_OBJ_G)
	$(CC) $(LDFLAGS) -o $(SERVER)-g $(SERVER_OBJ_G)

dgrep:$(CLIENT_OBJ)
	$(CC) $(LDFLAGS) -o $(CLIENT) $(CLIENT_OBJ)

dgrep-g:$(CLIENT_OBJ_G)
	$(CC) $(LDFLAGS) -o $(CLIENT)-g $(CLIENT_OBJ_G)

unittest:$(UNITTEST_OBJ)
	$(CC) $(LDFLAGS) -o $(UNITTEST) $(UNITTEST_OBJ)

unittest-g:$(UNITTEST_OBJ_G)
	$(CC) $(LDFLAGS) -o $(UNITTEST)-g $(UNITTEST_OBJ_G)

$(ODIR)/%.o: %.cpp %.h
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -c $<  -o $@

$(ODIR)/%.o-g: %.cpp %.h
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -c $<  -o $@

.PHONY: clean
clean: _ALL_OBJS=$(SERVER_OBJ) $(CLIENT_OBJ) $(UNITTEST_OBJ) 
clean: ALL_OBJS=$(_ALL_OBJS) $(_ALL_OBJS:%.o=%.o-g)
clean: ALL_BINS=$(BINS) $(BINS:%=%-g)
clean:
	@echo "Cleaning all..."
	rm -rf *.o $(ALL_OBJS) $(ALL_BINS)
