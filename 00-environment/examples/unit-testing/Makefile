CXX=g++

TARGET=tests
BUILD_DIR= build

CXXFLAGS= -c -std=c++17 -Wall -Wextra -O0 -g
LDFLAGS= -lCppUTest -lCppUTestExt

###########################################
#               INCLUDES
###########################################

INCLUDES+= -I .

############################################
#             SOURCE FILES
############################################

OBJS		+= $(BUILD_DIR)/main.o
OBJS		+= $(BUILD_DIR)/test_ring_buffer.o


PREREQ = $(BUILD_DIR)/

all: $(PREREQ) $(BUILD_DIR)/$(TARGET)

$(PREREQ):
	mkdir $(PREREQ)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(BUILD_DIR)/$(*).o $(*).cpp

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

run: all
	./$(BUILD_DIR)/$(TARGET) -c

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
