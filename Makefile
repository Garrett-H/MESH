# Compiler Version
CC=gcc

# Debugging flag -g
DEBUG=-g

# Target
TARGET=mesh

# Compile with all errors and warnings
CFLAGS=-c -Wall $(DEBUG)

# Makefile syntax:
# target: dependencies
#(tab) system command(s)

all: $(TARGET)

$(TARGET): main.o mesh.o
	$(CC) main.o mesh.o -o $(TARGET)

main.o: main.c mesh.h
	$(CC) $(CFLAGS) main.c

mesh.o: mesh.c mesh.h
	$(CC) $(CFLAGS) mesh.c

clean:
	rm *.o *~ $(TARGET)