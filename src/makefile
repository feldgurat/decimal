CC              := gcc
AR              := ar
ARFLAGS         := rcs
RM 				:= rm -rf

OS_NAME 		:= $(strip $(shell uname))
#CFLAGS          := -Wall -Wextra -Werror -std=c11 -D_POSIX_C_SOURCE=200809L
#CFLAGS          := -std=c11 -D_POSIX_C_SOURCE=200809L
ifeq ($(strip $(OS_NAME)),Linux)
    CFLAGS     	:= -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Werror
    LDFLAGS    	:=
else ifeq ($(strip $(OS_NAME)),Darwin)
    CFLAGS     	:= -std=c11 -D_POSIX_C_SOURCE=200809L -I/opt/homebrew/include -Wall -Wextra -Werror
    LDFLAGS    	:= -L/opt/homebrew/lib
endif

TEST_DIR		:= tests
REPORT_DIR		:= report
BUILD_DIR		:= build
OBJ_DIR			:= $(BUILD_DIR)/obj

LIB_NAME        := s21_decimal.a
LIB_PATH		:= $(BUILD_DIR)/$(LIB_NAME)

TEST_SRC 		:= $(wildcard $(TEST_DIR)/*.c)
TEST_BIN 		:= $(BUILD_DIR)/test_runner 

SRC         	:= $(wildcard *.c)
OBJ         	:= $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))

COV_INFO		:= $(BUILD_DIR)/coverage.info
COV_INFO_FILT	:= $(BUILD_DIR)/coverage_filtered.info

GCOV_FLAGS		:= --coverage

CHECK_CFLAGS	:= $(shell pkg-config --cflags check 2>/dev/null)
CHECK_LIBS		:= $(shell pkg-config --libs check 2>/dev/null)

CPPCHECK       	= cppcheck
CPPCHECK_BUILD 	= .cppcheck-build
CPPCHECK_FLAGS 	= --enable=all --std=c++11 --inconclusive \
            	  --suppress=missingIncludeSystem \
				  --suppress=unusedFunction \
				  --check-level=exhaustive \
                  --cppcheck-build-dir=$(CPPCHECK_BUILD) \
                  -j $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
SRC_DIR        	= ./

ifeq ($(strip $(CHECK_LIBS)),)
  ifeq ($(strip $(OS_NAME)),Linux)
    CHECK_LIBS := -lcheck -lm -lpthread -lrt -lsubunit
  else ifeq ($(strip $(OS_NAME)),Darwin)
    CHECK_LIBS := -lcheck
  endif
endif

OPEN_CMD := xdg-open
ifeq ($(strip $(OS_NAME)),Darwin)
  OPEN_CMD := open
endif

.PHONY: all clean test gcov_report open_report format s21_decimal.a memory_test check

all: clean $(LIB_PATH)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

s21_decimal.a: all
	cp $(LIB_PATH) ./

$(LIB_PATH): $(OBJ) | $(BUILD_DIR)
	$(AR) $(ARFLAGS) $@ $^ 

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(LIB_PATH)
	$(CC) $(CFLAGS) $(LDFLAGS) $(CHECK_CFLAGS) $(TEST_SRC) $(LIB_PATH) $(CHECK_LIBS) -o $(TEST_BIN)
	./$(TEST_BIN)

gcov_report: clean
	$(MAKE) test CFLAGS="$(CFLAGS) $(GCOV_FLAGS)"
	lcov --capture --directory . --output-file $(COV_INFO)
	lcov --remove $(COV_INFO) '*/$(TEST_DIR)/*' --output-file $(COV_INFO_FILT)
	genhtml $(COV_INFO_FILT) --output-directory $(REPORT_DIR)
	$(MAKE) open_report

open_report:
	@{ $(OPEN_CMD) $(REPORT_DIR)/index.html; } || \
	{ echo "$(REPORT_DIR)/index.html"; }

clean:
	$(RM) $(BUILD_DIR) $(REPORT_DIR) s21_decimal.a $(CPPCHECK_BUILD)

format:
	clang-format -n tests/*.c
	clang-format -n *.c
	clang-format -n *.h

memory_test: clean $(LIB_PATH)
	$(CC) $(CFLAGS) $(LDFLAGS) $(CHECK_CFLAGS) $(TEST_SRC) $(LIB_PATH) $(CHECK_LIBS) -o $(TEST_BIN)
	if [ "$(OS_NAME)" = "Linux" ]; then \
		valgrind --tool=memcheck --leak-check=yes $(TEST_BIN); \
	elif [ "$(OS_NAME)" = "Darwin" ]; then \
		leaks -atExit -- $(TEST_BIN); \
	fi


check:
	@mkdir -p $(CPPCHECK_BUILD)
	$(CPPCHECK) $(CPPCHECK_FLAGS) $(SRC_DIR)