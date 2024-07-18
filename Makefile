CC = g++
RM ?= rm -f

SRC_DIR = source
OBJ_DIR = build
INCLUDE_DIR = include
TESTS_DIR = tests
BIN_DIR = .

MAIN = $(SRC_DIR)/main.cpp
MAINOBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(MAIN))

SRC = 

SRCOBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

LIBSRC = 
LIBOBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(LIBSRC))

SRC_TEST = 
OBJ_TEST = $(patsubst $(TESTS_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_TEST))

CFLAGS = -Wall -Wextra
LDLIBS = -lm
CPPFLAGS = -iquote ./$(INCLUDE_DIR)

CRITFLAGS = -lcriterion
GCOVRFLAGS = --coverage

NAME = $(BIN_DIR)/shukascript
TEST_NAME = $(BIN_DIR)/unit_tests

all: $(NAME)

$(NAME): $(LIBOBJ) $(MAINOBJ) $(SRCOBJ) | $(BIN_DIR)
	$(CC) $(LIBOBJ) $(MAINOBJ) $(SRCOBJ) -o $(NAME) \
	$(CFLAGS) $(LDLIBS) $(CPPFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)/*.o

fclean: clean
	$(RM) $(NAME) $(TEST_NAME)

re: fclean all

debug: CFLAGS += -ggdb3
debug: re

asan: CC = clang -fsanitize=address
asan: re

$(TEST_NAME): $(OBJ_TEST) $(LIBOBJ) $(SRCOBJ) | $(BIN_DIR)
	$(CC) -o $(TEST_NAME) $(CFLAGS) $(CPPFLAGS) \
	$(OBJ_TEST) $(SRCOBJ) $(LIBOBJ) $(LDFLAGS) $(LDLIB) \
	$(CRITFLAGS) $(GCOVRFLAGS) $(LDLIBS)

tests_run: $(TEST_NAME)
	./$(TEST_NAME)

tests_clean:
	$(RM) $(OBJ_DIR)/*.gcda
	$(RM) $(OBJ_DIR)/*.gcno
	$(RM) coverage*
	$(RM) $(TEST_NAME)

coverage:
	gcovr --html-details coverage && firefox coverage

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

.PHONY: all clean fclean re debug asan tests_run coverage tests_clean
