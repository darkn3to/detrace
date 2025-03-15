CXX = g++
CXXFLAGS = -Iinclude -Wall -std=c++17

TARGET = detrace

SRC = lexer.cpp
OBJ = $(SRC:.cpp=.o)

.PHONY: all clean

all: clean $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp include/log.hpp include/file_io.hpp include/token.hpp include/fsm.hpp include/all.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
