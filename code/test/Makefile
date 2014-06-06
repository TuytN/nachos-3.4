# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as

GCCDIR = ../../../cross-compiler/decstation-ultrix/bin/

LDFLAGS = -T script -N

ASFLAGS = -mips2

CPPFLAGS = $(INCDIR)

# if you aren't cross-compiling:

# GCCDIR =

# LDFLAGS = -N -T 0

# ASFLAGS =

# CPPFLAGS = -P $(INCDIR)

PATH = $(GCCDIR):/lib:/usr/bin:/bin

CC = $(GCCDIR)gcc -B../../../cross-compiler/

AS = $(GCCDIR)as

LD = $(GCCDIR)ld

CPP = gcc -E

INCDIR =-I../userprog -I../threads

CFLAGS = -G 0 -c $(INCDIR)

all: halt shell matmult sort inttest sorttest help ascii stringtest chartest createfile echo reverse cat copy

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.c > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
shell: shell.o start.o
	$(LD) $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult


inttest.o: inttest.c
	$(CC) $(CFLAGS) -c inttest.c
inttest: inttest.o start.o
	$(LD) $(LDFLAGS) start.o inttest.o -o inttest.coff
	../bin/coff2noff inttest.coff inttest

sorttest.o: sorttest.c
	$(CC) $(CFLAGS) -c sorttest.c
sorttest: sorttest.o start.o
	$(LD) $(LDFLAGS) start.o sorttest.o -o sorttest.coff
	../bin/coff2noff sorttest.coff sorttest

help.o: help.c
	$(CC) $(CFLAGS) -c help.c
help: help.o start.o
	$(LD) $(LDFLAGS) start.o help.o -o help.coff
	../bin/coff2noff help.coff help

ascii.o: ascii.c
	$(CC) $(CFLAGS) -c ascii.c
ascii: ascii.o start.o
	$(LD) $(LDFLAGS) start.o ascii.o -o ascii.coff
	../bin/coff2noff ascii.coff ascii

stringtest.o: stringtest.c
	$(CC) $(CFLAGS) -c stringtest.c
stringtest: stringtest.o start.o
	$(LD) $(LDFLAGS) start.o stringtest.o -o stringtest.coff
	../bin/coff2noff stringtest.coff stringtest

chartest.o: chartest.c
	$(CC) $(CFLAGS) -c chartest.c
chartest: chartest.o start.o
	$(LD) $(LDFLAGS) start.o chartest.o -o chartest.coff
	../bin/coff2noff chartest.coff chartest

createfile.o: createfile.c
	$(CC) $(CFLAGS) -c createfile.c
createfile: createfile.o start.o
	$(LD) $(LDFLAGS) start.o createfile.o -o createfile.coff
	../bin/coff2noff createfile.coff createfile

echo.o: echo.c
	$(CC) $(CFLAGS) -c echo.c
echo: echo.o start.o
	$(LD) $(LDFLAGS) start.o echo.o -o echo.coff
	../bin/coff2noff echo.coff echo

reverse.o: reverse.c
	$(CC) $(CFLAGS) -c reverse.c
reverse: reverse.o start.o
	$(LD) $(LDFLAGS) start.o reverse.o -o reverse.coff
	../bin/coff2noff reverse.coff reverse

cat.o: cat.c
	$(CC) $(CFLAGS) -c cat.c
cat: cat.o start.o
	$(LD) $(LDFLAGS) start.o cat.o -o cat.coff
	../bin/coff2noff cat.coff cat

copy.o: copy.c
	$(CC) $(CFLAGS) -c copy.c
copy: copy.o start.o
	$(LD) $(LDFLAGS) start.o copy.o -o copy.coff
	../bin/coff2noff copy.coff copy
