CC=gcc
CFLAGS= -Wall -std=c99 -g
TARGET=run
FILES=./src/main.c ./src/roomgen.c ./src/entities.c ./src/driver.c
INCLUDE=./include
LINKS= -lncurses

.PHONY: all clean

all: $(TARGET)
   
$(TARGET): $(FILES) $(wildcard include/*)
	$(CC) $(CFLAGS) -o $(TARGET) $(FILES) -I $(INCLUDE) $(LINKS)
	
clean:
	rm $(TARGET)

pseudo:
	gcc -Wall -std=c99 -g -o run ./src/main.c ./src/roomgen.c ./src/entities.c ./src/driver.c -I./include -lncurses 
