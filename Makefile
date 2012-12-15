#
# Created by gmakemake (Ubuntu Sep  7 2011) on Sat Dec 15 15:23:01 2012
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

#
# This header.mak file will set up all necessary options for compiling
# and linking C programs for SP1 which use the "allocate" library.
#
# To use it, place it in the same directory as your source code, and
# run the command
#
#       gmakemake > Makefile
#
# To compile and link your program, just run "make".
#
ALLOCDIR =	/usr/local/pub/wrc/courses/sp1/allocate
CC =		gcc
CFLAGS =	-ggdb -std=c99 -I$(ALLOCDIR)
LIBFLAGS =	-L$(ALLOCDIR) -lallocate
CLIBFLAGS =	$(LIBFLAGS)

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	ExecuteCommands.c LinkedDataNodeOperations.c SanitizeInput.c project1.c
PS_FILES =	
S_FILES =	
H_FILES =	ExecuteCommands.h LinkedDataNodeOperations.h LinkedDataNodeStructures.h SanitizeInput.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	ExecuteCommands.o LinkedDataNodeOperations.o SanitizeInput.o 

#
# Main targets
#

all:	project1 

project1:	project1.o $(OBJFILES)
	$(CC) $(CFLAGS) -o project1 project1.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

ExecuteCommands.o:	ExecuteCommands.h LinkedDataNodeOperations.h LinkedDataNodeStructures.h
LinkedDataNodeOperations.o:	LinkedDataNodeOperations.h LinkedDataNodeStructures.h
SanitizeInput.o:	SanitizeInput.h
project1.o:	ExecuteCommands.h LinkedDataNodeOperations.h LinkedDataNodeStructures.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm $(OBJFILES) project1.o core 2> /dev/null

realclean:        clean
	-/bin/rm -rf project1 
