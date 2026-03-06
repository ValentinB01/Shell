CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lreadline
SRCS = src/main.c src/parser.c src/executor.c src/history.c src/signals.c src/alias.c
OBJS = $(SRCS:.c=.o)
TARGET = myshell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)