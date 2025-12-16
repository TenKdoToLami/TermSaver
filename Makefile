CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -D_XOPEN_SOURCE_EXTENDED -Iinclude
LDFLAGS = -lncursesw

SRC_DIR = src
OBJ_DIR = obj
BIN = ascii_screensaver

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: all
	./$(BIN)

clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all run clean
