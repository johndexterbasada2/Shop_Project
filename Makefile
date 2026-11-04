CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
SRC      = $(wildcard BlitzShop/*.cpp)
TARGET   = BlitzShop.exe

all: $(TARGET)

$(TARGET): $(SRC) $(wildcard BlitzShop/*.h)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	-del $(TARGET)

.PHONY: all run clean
