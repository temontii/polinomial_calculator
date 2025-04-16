# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lm

# Source files
SRCS = main.c ui.c Polynomial.c Integer.c Complex.c tests.c
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = polynomial_calculator

# Header files
HEADERS = ui.h Polynomial.h Integer.h Complex.h TypeInfo.h PolynomialDefines.h tests.h

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	./$(TARGET) --test

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)