CXX = g++
CXXFLAGS = -Iinclude -Wall -std=c++17

TARGET = a.out

SRC = main.cpp file_io.cpp
OBJ = $(SRC:.cpp=.o)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp include/file_io.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
