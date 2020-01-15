#
# Created by gmakemake (Ubuntu Jul 25 2014) on Thu Mar 14 00:04:08 2019
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

CFLAGS = -ggdb -std=c99 -Wall -Wextra -pedantic

CLIBFLAGS = -lm

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	HeapADT.c test_HeapADT.c
PS_FILES =	
S_FILES =	
H_FILES =	HeapADT.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	HeapADT.o 

#
# Main targets
#

all:	test_HeapADT 

test_HeapADT:	test_HeapADT.o $(OBJFILES)
	$(CC) $(CFLAGS) -o test_HeapADT test_HeapADT.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

HeapADT.o:	HeapADT.h
test_HeapADT.o:	HeapADT.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) test_HeapADT.o core

realclean:        clean
	-/bin/rm -f test_HeapADT 
