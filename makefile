CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -I./src
LDFLAGS := -pthread

# Directory structure
SRC_DIR := src
TEST_DIR := tests
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
COV_DIR := $(BUILD_DIR)/coverage
MEMCHECK_DIR := $(BUILD_DIR)/memcheck

# Source files
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Test files
TEST_SRCS := $(shell find $(TEST_DIR) -name '*.cpp')
TEST_OBJS := $(TEST_SRCS:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/$(TEST_DIR)/%.o)
TEST_DEPS := $(TEST_OBJS:.o=.d)

# Executables
EXEC := $(BIN_DIR)/mst_project
TEST_EXEC := $(BIN_DIR)/test_mst_project

# Add Google Test
GTEST_DIR := /usr/local/include/gtest
GTEST_LIB := /usr/local/lib
CXXFLAGS += -I$(GTEST_DIR)/include
LDFLAGS += -L$(GTEST_LIB) -lgtest -lgtest_main

# Valgrind command
VALGRIND := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

.PHONY: all clean test coverage profile run_all memcheck

all: $(EXEC) $(TEST_EXEC)

$(EXEC): $(filter-out $(OBJ_DIR)/tests/%, $(OBJS))
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(TEST_EXEC): $(filter-out $(OBJ_DIR)/main.o, $(OBJS)) $(TEST_OBJS)
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -MMD -MP -c $< -o $@

-include $(DEPS) $(TEST_DEPS)

test: $(TEST_EXEC)
	./$(TEST_EXEC)

coverage: CXXFLAGS += -fprofile-arcs -ftest-coverage
coverage: LDFLAGS += -lgcov
coverage: clean test
	@mkdir -p $(COV_DIR)
	find $(OBJ_DIR) -name "*.gcno" -exec gcov -r {} +
	lcov --capture --directory $(OBJ_DIR) --output-file $(COV_DIR)/coverage.info
	genhtml $(COV_DIR)/coverage.info --output-directory $(COV_DIR)/report

profile: CXXFLAGS += -pg
profile: LDFLAGS += -pg
profile: clean $(EXEC)
	./$(EXEC)
	gprof $(EXEC) gmon.out > $(BUILD_DIR)/profile_report.txt

memcheck: CXXFLAGS += -g
memcheck: clean $(TEST_EXEC)
	@mkdir -p $(MEMCHECK_DIR)
	$(VALGRIND) --log-file=$(MEMCHECK_DIR)/memcheck.log $(TEST_EXEC)
	@echo "Memcheck complete. Results saved to $(MEMCHECK_DIR)/memcheck.log"

run_all: $(EXEC)
	./run_all_features.sh

clean:
	rm -rf $(BUILD_DIR)