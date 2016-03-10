CC		=gcc
CFLAGS	= -Wall -std=c99 -g
EXEC	=run
OBJECTS	=main.o roomgen.o entities.o driver.o
FILES	=./src/main.c ./src/roomgen.c ./src/entities.c ./src/driver.c
INCLUDE	=./include
LIB		= -lncurses

.PHONY: all clean

all: $(EXEC)

$(EXEC): main.o roomgen.o entities.o driver.o
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJECTS) -I $(INCLUDE) $(LIB)

main.o: ./src/main.c $(wildcard include/*.h)
	$(CC) -c $(CFLAGS) -o $@ ./src/main.c -I $(INCLUDE)

roomgen.o: ./src/roomgen.c $(wildcard include/*.h)
	$(CC) -c $(CFLAGS) -o $@ ./src/roomgen.c -I $(INCLUDE)

entities.o: ./src/entities.c $(wildcard include/*.h)
	$(CC) -c $(CFLAGS) -o $@ ./src/entities.c -I $(INCLUDE)
	
driver.o: ./src/driver.c $(wildcard include/*.h)
	$(CC) -c $(CFLAGS) -o $@ ./src/driver.c -I $(INCLUDE)
	
clean:
	rm $(EXEC) $(OBJECTS)
