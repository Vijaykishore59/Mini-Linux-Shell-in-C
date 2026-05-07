CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude
SRC = src/main.c src/shell.c src/parser.c src/executor.c src/history.c src/utils.c
OBJ = $(SRC:.c=.o)
TARGET = mini_shell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
