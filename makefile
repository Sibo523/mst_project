CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -I./src
LDFLAGS := -pthread

SRC_DIR := src
BUILD_DIR := build
TEST_DIR := tests

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

TEST_SRCS := $(shell find $(TEST_DIR) -name '*.cpp')
TEST_OBJS := $(TEST_SRCS:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/$(TEST_DIR)/%.o)
TEST_DEPS := $(TEST_OBJS:.o=.d)

EXEC := mst_project
TEST_EXEC := test_mst_project

.PHONY: all clean test coverage profile run_all

all: $(EXEC)

$(EXEC): $(filter-out $(BUILD_DIR)/main.o, $(OBJS)) $(BUILD_DIR)/main.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): $(filter-out $(BUILD_DIR)/main.o, $(OBJS)) $(TEST_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -MMD -MP -c $< -o $@

-include $(TEST_DEPS)

coverage: CXXFLAGS += -fprofile-arcs -ftest-coverage
coverage: LDFLAGS += -lgcov
coverage: clean test
	gcov -r $(SRCS)
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report

profile: CXXFLAGS += -pg
profile: LDFLAGS += -pg
profile: clean $(EXEC)
	./$(EXEC)
	gprof $(EXEC) gmon.out > profile_report.txt

run_all: $(EXEC)
	./run_all_features.sh

clean:
	rm -rf $(BUILD_DIR) $(EXEC) $(TEST_EXEC) *.gcda *.gcno *.gcov coverage.info coverage_report profile_report.txt gmon.out

-include $(wildcard $(BUILD_DIR)/*.d)
