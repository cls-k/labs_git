CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -fPIC
INCLUDE = -Iinclude
LIB_SRC = src/int_set.c
LIB_OBJ = src/int_set.o
LIB_STATIC = libint_set.a
LIB_SHARED = libint_set.so

all: $(LIB_STATIC) $(LIB_SHARED)

$(LIB_SHARED): $(LIB_OBJ)
	$(CC) -shared -o $@ $^

$(LIB_STATIC): $(LIB_OBJ)
	ar rcs $@ $^

$(LIB_OBJ): $(LIB_SRC) include/int_set.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

test: $(LIB_STATIC)
	$(CC) $(CFLAGS) $(INCLUDE) tests/test.c -L. -lint_set -o tests/test
	./tests/test

py-test: $(LIB_SHARED)
	PYTHONPATH=. python3 tests/test_int_set.py

clean:
	rm -f $(LIB_OBJ) $(LIB_STATIC) $(LIB_SHARED) tests/test

.PHONY: all clean test py-test
