CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -I./src
LDFLAGS := -pthread

# Directory structure
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
COV_DIR := $(BUILD_DIR)/coverage
MEMCHECK_DIR := $(BUILD_DIR)/memcheck

# Source files (excluding main.cpp and test files)
SRCS := $(filter-out $(SRC_DIR)/main.cpp,$(shell find $(SRC_DIR) -name '*.cpp'))
TEST_SRCS := $(shell find $(SRC_DIR)/tests -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJS := $(TEST_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
MAIN_OBJ := $(OBJ_DIR)/main.o
DEPS := $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

# Executables
EXEC := mst_project
TEST_EXEC := $(BIN_DIR)/test_mst_project

# Add Google Test
GTEST_DIR := /usr/local/include/gtest
GTEST_LIB := /usr/local/lib
CXXFLAGS += -I$(GTEST_DIR)/include
LDFLAGS += -L$(GTEST_LIB) -lgtest -lgtest_main

# Valgrind commands
VALGRIND := valgrind --leak-check=full -s --show-leak-kinds=all --track-origins=yes
HELGRIND := valgrind --tool=helgrind --trace-children=yes
CALLGRIND := valgrind --tool=callgrind 
.PHONY: all clean test coverage memcheck helgrind

all: $(EXEC) $(TEST_EXEC)

$(EXEC): $(OBJS) $(MAIN_OBJ)
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(TEST_EXEC): $(OBJS) $(TEST_OBJS)
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(MAIN_OBJ): $(SRC_DIR)/main.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

test: $(TEST_EXEC)
	./$(TEST_EXEC)


memcheck: CXXFLAGS += -g
memcheck:  $(TEST_EXEC)
	@mkdir -p $(MEMCHECK_DIR)
	$(VALGRIND) --log-file=$(MEMCHECK_DIR)/memcheck.log ./$(TEST_EXEC)
	@echo "Memcheck complete. Results saved to $(MEMCHECK_DIR)/memcheck.log"
 
helgrind: CXXFLAGS += -g
helgrind: $(TEST_EXEC)
	@mkdir -p $(MEMCHECK_DIR)
	$(HELGRIND) --log-file=$(MEMCHECK_DIR)/helgrind.log ./$(TEST_EXEC)
cg:  $(TEST_EXEC)
	@mkdir -p $(MEMCHECK_DIR)
	$(CALLGRIND) --callgrind-out-file=$(MEMCHECK_DIR)/callgrind.log ./$(TEST_EXEC)
	callgrind_annotate  $(MEMCHECK_DIR)/callgrind.log > $(MEMCHECK_DIR)/callgrind_annotate.log
 
clean:
	rm -rf $(BUILD_DIR)
	find . -type f -name "*.gcno" -delete
	find . -type f -name "*.gcda" -delete
	find . -type f -name "*.gcov" -delete
	rm -f $(EXEC) gmon.out