CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
TARGET = inventory

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe inventory.dat stock_history.txt

.PHONY: all clean
