CC := gcc
CFLAGS := -Wall -Wextra -Werror
TARGET := num2shu
BIN := ./bin
SRC := ./src
TEST := ./test
TEST_BIN := ./test_bin

DEBUG_FLAGS := -g -O0
RELEASE_FLAGS := -O3

BUILD ?= release
ifeq ($(BUILD),debug)
    CFLAGS += $(DEBUG_FLAGS)
else
    CFLAGS += $(RELEASE_FLAGS)
endif

BIN := ./bin/$(BUILD)

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

$(BIN)/$(TARGET): $(SRC)/$(TARGET).c
	mkdir -p $(BIN)
	$(CC) $(CFLAGS) $^ -o $@

install: $(BIN)/$(TARGET)
	install -d $(PREFIX)/bin
	install $(BIN)/$(TARGET) $(PREFIX)/bin

uninstall: $(PREFIX)/bin/$(TARGET)
	rm -f $(PREFIX)/bin/$(TARGET)

$(TEST_BIN)/$(TARGET)_test: $(TEST)/$(TARGET)_test.c $(TEST)/$(TARGET)_test_cases.c
	mkdir -p $(TEST_BIN)
	$(CC) $(CFLAGS) $^ -o $@

test: $(BIN)/$(TARGET) $(TEST_BIN)/$(TARGET)_test
	$(TEST_BIN)/$(TARGET)_test $(BIN)/$(TARGET)

clean:
	rm -rf ./bin
	rm -rf $(TEST_BIN)

.PHONY: install uninstall test clean

