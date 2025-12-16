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
	g++  obj/BouncingAsciiLogo.o obj/RippleAsciiLogo.o obj/HeartbeatAsciiLogo.o obj/Logo.o  obj/Menu.o  obj/RotatingLineLogo.o obj/AsciiArt.o obj/main.o -o ascii_screensaver -lncursesw

$(OBJ_DIR)/BouncingAsciiLogo.o: $(SRC_DIR)/BouncingAsciiLogo.cpp | $(OBJ_DIR)
	g++ -Wall -Wextra -std=c++17 -D_XOPEN_SOURCE_EXTENDED -Iinclude -c src/BouncingAsciiLogo.cpp -o obj/BouncingAsciiLogo.o
$(OBJ_DIR)/RippleAsciiLogo.o: $(SRC_DIR)/RippleAsciiLogo.cpp | $(OBJ_DIR)
	g++ -Wall -Wextra -std=c++17 -D_XOPEN_SOURCE_EXTENDED -Iinclude -c src/RippleAsciiLogo.cpp -o obj/RippleAsciiLogo.o
$(OBJ_DIR)/HeartbeatAsciiLogo.o: $(SRC_DIR)/HeartbeatAsciiLogo.cpp | $(OBJ_DIR)
	g++ -Wall -Wextra -std=c++17 -D_XOPEN_SOURCE_EXTENDED -Iinclude -c src/HeartbeatAsciiLogo.cpp -o obj/HeartbeatAsciiLogo.o
$(OBJ_DIR)/Logo.o: $(SRC_DIR)/Logo.cpp | $(OBJ_DIR)
	g++ -Wall -Wextra -std=c++17 -D_XOPEN_SOURCE_EXTENDED -Iinclude -c src/Logo.cpp -o obj/Logo.o
$(OBJ_DIR)/AsciiArt.o: $(SRC_DIR)/AsciiArt.cpp | $(OBJ_DIR)
	g++ -Wall -Wextra -std=c++17 -D_XOPEN_SOURCE_EXTENDED -Iinclude -c src/AsciiArt.cpp -o obj/AsciiArt.o
$(OBJ_DIR)/Menu.o: $(SRC_DIR)/Menu.cpp | $(OBJ_DIR)
	g++ -Wall -Wextra -std=c++17 -D_XOPEN_SOURCE_EXTENDED -Iinclude -c src/Menu.cpp -o obj/Menu.o
$(OBJ_DIR)/RotatingLineLogo.o: $(SRC_DIR)/RotatingLineLogo.cpp | $(OBJ_DIR)
	g++ -Wall -Wextra -std=c++17 -D_XOPEN_SOURCE_EXTENDED -Iinclude -c src/RotatingLineLogo.cpp -o obj/RotatingLineLogo.o
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp | $(OBJ_DIR)
	g++ -Wall -Wextra -std=c++17 -D_XOPEN_SOURCE_EXTENDED -Iinclude -c src/main.cpp -o obj/main.o

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: all
	./$(BIN)

clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all run clean
