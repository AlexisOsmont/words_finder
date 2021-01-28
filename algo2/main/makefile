hashtabledir = ../hashtable/
holdalldir = ../holdall/
listdyndir = ../listdyn/
list_linedir = ../list_line/
word_implantationdir = ../word_implantation/
CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -DHOLDALL_INSERT_TAIL -g -Wextra -Wpedantic -I$(hashtabledir) -I$(holdalldir) -I$(word_implantationdir) -I$(listdyndir) -I$(list_linedir) -O2
LDFLAGS = 
VPATH = $(hashtabledir):$(holdalldir):$(listdyndir):$(list_linedir):$(word_implantationdir)
objects = $(hashtabledir)hashtable.o $(holdalldir)holdall.o $(listdyndir)listdyn.o $(list_linedir)list_line.o $(word_implantationdir)word_implantation.o lidx.o
executable = lidx

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

$(hashtabledir)hashtable.o: hashtable.c hashtable.h
$(holdalldir)holdall.o: holdall.c holdall.h
$(listdyndir)listdyn.o: listdyn.c listdyn.h
$(list_linedir)list_line.o: list_line.c list_line.h
$(word_implantationdir)word_implantation.o: word_implantation.c word_implantation.h
lidx.o: lidx.c hashtable.h holdall.h listdyn.h list_line.h word_implantation.h
