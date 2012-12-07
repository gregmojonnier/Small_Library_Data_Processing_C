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
