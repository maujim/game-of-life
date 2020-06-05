# Makefile

TARGET_EXEC ?= game

CXX = g++
CXXFLAGS = -g -Wall -Og -std=c++17

all: clean game.cpp
	$(CXX) $(CXXFLAGS) game.cpp -o $(TARGET_EXEC)

.PHONY: clean
clean:
	rm -f $(TARGET_EXEC)
