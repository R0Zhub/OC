CC = gcc
CFLAGS = -Wall -Wextra -O3 -pthread
LDFLAGS = -pthread -lm

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

SRCS = main.c $(SRC_DIR)/sequential.c $(SRC_DIR)/parallel.c
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
TARGET = lab2

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/src

$(BUILD_DIR)/main.o: main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean