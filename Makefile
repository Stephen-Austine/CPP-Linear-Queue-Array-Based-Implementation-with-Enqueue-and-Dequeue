# Makefile - linear_queue
CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic -O2

SRC    := src/linear_queue.cpp
TARGET := bin/linear_queue
TEST_BIN := bin/test_linear_queue

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(SRC) | bin
	$(CXX) $(CXXFLAGS) -o $@ $<

bin:
	mkdir -p bin

run: $(TARGET)
	./$(TARGET)

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): tests/test_linear_queue.cpp $(SRC) | bin
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -rf bin
