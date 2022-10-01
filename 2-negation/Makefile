# https://courses.cs.washington.edu/courses/cse451/99wi/Section/makeintro.html

TARGET = main
SRC = ../libcompact.c
OBJ = $(SRC:.c=.o)
INCLUDE = -I.

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -lm

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

all: $(TARGET)

$(TARGET): $(OBJ) $(TARGET).c
	$(CC) -o $(TARGET) $(TARGET).c $(INCLUDE) $(OBJ) $(CFLAGS) 


clean:
	rm $(TARGET) $(OBJ) $(TARGET).o
