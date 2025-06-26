SRC = Token_Utils.cpp Vakya_Lexer.cpp Vakya_Prompt.cpp Compiler.cpp
CXX = g++
FLAGS = -Wall -Wextra -std=c++17
DBG_FLG = -g -Wall -Wextra -std=c++17
OBJ = $(SRC:.cpp=.o)
TARGET = Vakya
DEBUG_TARGET = $(TARGET)_DBG

ifeq ($(BUILD), debug)
	FLAGS = $(DBG_FLG)
	TARGET = Vakya_DBG
endif

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(FLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(FLAGS) -c $< -o $@

clean: 
	rm -f $(OBJ) $(TARGET)

	
