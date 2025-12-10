CC = gcc
CFLAGS = -Wall -g
SRCS = src/main.c src/parser.c src/executor.c src/history.c src/signals.c
OBJS = $(SRCS:.c=.o)
TARGET = myshell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)