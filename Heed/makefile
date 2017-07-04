#OBJDIR = $(GARFIELD_HOME)/Object
#SRCDIR = $(GARFIELD_HOME)/Source
#INCDIR = $(GARFIELD_HOME)/Include
#HEEDDIR = $(GARFIELD_HOME)/Heed
#LIBDIR = $(GARFIELD_HOME)/Library
#
## Compiler flags
#CFLAGS = -Wall -Wextra -Wno-long-long \
#	`root-config --cflags` \
#	-O3 -fno-common -c \
#	-I$(INCDIR) -I$(HEEDDIR)
#
## Debug flags
##CFLAGS += -g
#
#LDFLAGS = -L$(LIBDIR) -lGarfield
#LDFLAGS += `root-config --glibs` -lGeom -lgfortran -lm
##LDFLAGS += -g
#
#edep: edep.C 
#	$(CXX) $(CFLAGS) edep.C
#	$(CXX) -o edep edep.o $(LDFLAGS)
#	rm edep.o

# CORTO data analysis makefile
EXE = edep
CC = g++

OBJDIR = $(GARFIELD_HOME)/Object
SRCDIR = $(GARFIELD_HOME)/Source
INCDIR = $(GARFIELD_HOME)/Include
HEEDDIR = $(GARFIELD_HOME)/Heed
LIBDIR = $(GARFIELD_HOME)/Library

################
# Sources
#SOURCES_CPP += CORTOEvent.cpp
#SOURCES_CPP += main.cpp
SOURCES_CPP = $(wildcard *.cpp)

SOURCES = $(SOURCES_CPP)
OBJECTS = $(SOURCES_CPP:.cpp=.o)

######################
# Includes and Defines
INCLUDES += -I .
INCLUDES += -I$(shell root-config --incdir)
INCLUDES += -I$(INCDIR)
INCLUDES += -I$(HEEDDIR)

ROOTFLAGS = $(shell root-config --libs --glibs --cflags --ldflags --auxlibs --auxcflags)

DEBUG_FLAGS = -g -O0 -gdwarf-2
#DEBUGFLAGS = -O2

CPPFLAGS += $(DEFINES) $(DEBUG_FLAGS) $(CPPFLAGS_EXTRA) $(INCLUDES) $(ROOTFLAGS)
CPPFLAGS += -c -Wall

LDFLAGS = -L$(LIBDIR) -lGarfield
LDFLAGS += `root-config --glibs` -lGeom -lgfortran -lm
#LDFLAGS += -g

################
# Build rules

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CPPFLAGS) $< -o $@
	
clean:
	$(RM) $(OBJECTS) $(EXE) 
