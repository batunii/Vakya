
SRC = Token_Utils.cpp Vakya_Lexer.cpp Compiler.cpp
CXX = g++
FLAGS = -Wall -Wextra -std=c++17
OBJ = $(SRC:.cpp=.o)
TARGET = Vakya

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(FLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(FLAGS) -c $< -o $@

clean: 
	rm -f $(OBJ) $(TARGET)

	
