########################################################################
# COSC1076 - Advanced Programming Techniques
# Semester 2 2014 Assignment #2
# Full Name        : Tuan Minh Tran
# Student Number   : s3488978
# Course Code      : COSC1076
# Program Code     : BP094
# Start up code provided by Paul Miller
########################################################################

CC = gcc
CFLAGS = -ansi -Wall -pedantic
LFLAGS=

SOURCES=ppd_main.c ppd_menu.c ppd_options.c ppd_utility.c ppd_stock.c \
ppd_coin.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS=ppd_main.h ppd_menu.h ppd_options.h ppd_utility.h ppd_stock.h \
ppd_coin.h
README=ppd_readme
MAKEFILE=Makefile

########################################################################
# Move this target to the end of the Makefile to zip up your code 
# when submitting. Do not submit your .dat files, or directories. 
# We only want the files that are part of your implementation.
########################################################################
all:ppd

ppd: $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o ppd

%.o:%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

debug:CFLAGS=-ansi -Wall -pedantic -g
debug:clean ppd

.PHONY:clean
clean:
	rm -f *.o ppd

archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) $(README) $(MAKEFILE)
