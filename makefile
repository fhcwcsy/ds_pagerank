# CC and CFLAGS are varilables
CC = g++
CFLAGS = -c
AR = ar
ARFLAGS = rcv
# -c option ask g++ to compile the source files, but do not link.
# -O2 option is for optimized version
OPTFLAGS = -O3

all	: bin/pagerank
	@echo -n ""

# optimized version
bin/pagerank: main.o pagerank.o
			$(CC) $(OPTFLAGS) main.o pagerank.o -o bin/pagerank
main.o 	   	: src/main.cpp
			$(CC) $(CFLAGS) $< -o $@
pagerank.o	: src/pagerank.cpp src/pagerank.h
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# clean all the .o, executable files, outputs, and axilliary files of report
clean:
		rm -rf *.o *.txt bin/* ./docs/*.aux ./docs/*.log

