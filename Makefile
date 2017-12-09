
# determine board
ifeq ($(host),pc)
	CROSS_COMPILE=
	ROOT_PATH=
else
	CROSS_COMPILE=ppc_6xx-
	ROOT_PATH=
endif

DEFINES=

CLIENT_OUTPUT=cryptoenc
SERVER_OUTPUT=cryptodec

# shell directives
CXX = g++
CC = gcc
AR = ar
CP = cp

# determine source codes to be compiled
MAIN_C_CLIENT_SOURCES=$(shell find src -name "*.c" | grep -v src/main_dec.c)
MAIN_CPP_CLIENT_SOURCES=$(shell find src -name "*.cpp")
MAIN_C_CLIENT_OBJECTS=$(MAIN_C_CLIENT_SOURCES:.c=.o)
MAIN_CPP_CLIENT_OBJECTS=$(MAIN_CPP_CLIENT_SOURCES:.cpp=.o)

MAIN_C_SERVER_SOURCES=$(shell find src -name "*.c" | grep -v src/main_enc.c)
MAIN_CPP_SERVER_SOURCES=$(shell find src -name "*.cpp")
MAIN_C_SERVER_OBJECTS=$(MAIN_C_SERVER_SOURCES:.c=.o)
MAIN_CPP_SERVER_OBJECTS=$(MAIN_CPP_SERVER_SOURCES:.cpp=.o)

MAIN_APP_CLIENT_OBJECTS=${MAIN_C_CLIENT_OBJECTS} ${MAIN_CPP_CLIENT_OBJECTS}
MAIN_APP_SERVER_OBJECTS=${MAIN_C_SERVER_OBJECTS} ${MAIN_CPP_SERVER_OBJECTS}

INCLUDE= -Isrc/

LINK= -lcrypto -lssl

CFLAGS= -Wall

CPPFLAGS= -Wall

all : $(CLIENT_OUTPUT)  $(SERVER_OUTPUT)

${CLIENT_OUTPUT}: ${MAIN_APP_CLIENT_OBJECTS}
	$(CROSS_COMPILE)$(CC) ${MAIN_APP_CLIENT_OBJECTS} $(DEFINES) -o ${CLIENT_OUTPUT} $(LINK)

${SERVER_OUTPUT}: ${MAIN_APP_SERVER_OBJECTS}
	$(CROSS_COMPILE)$(CC) ${MAIN_APP_SERVER_OBJECTS} $(DEFINES) -o ${SERVER_OUTPUT} $(LINK)

%.o : %.c 
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(INCLUDE) $(DEFINES) -c $< -o $@
 
%.o : %.cpp
	$(CROSS_COMPILE)$(CXX) $(CPPFLAGS) $(INCLUDE) $(DEFINES) -c $< -o $@

clean:
	rm -f `find src -name "*.o"` $(CLIENT_OUTPUT) $(SERVER_OUTPUT)

