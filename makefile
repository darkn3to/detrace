CXX = g++
CXXFLAGS = -Iinclude -Wall -std=c++17

TARGET = detrace

SRC = lexer.cpp file_io.cpp
OBJ = $(SRC:.cpp=.o)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp include/file_io.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp include/token.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp include/all.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
