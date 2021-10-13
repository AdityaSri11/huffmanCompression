CC = gcc
CFLAGS = -g -std=c11 -Wall -Wshadow -Wvla -Werror -pedantic -Wno-unused-function
BASE_NAME = huffman
SRC_C = huffman.c priority_queue.c frequencies.c bit_writer.c
TEST_C = test_$(BASE_NAME).c
ASG_NICKNAME = HW20
SRC_H = huffman.h priority_queue.h frequencies.h miniunit.h clog.h bit_writer.h 
TEST_EXPECTED = expected.txt
SUBMIT_FILES = $(SRC_C) $(TEST_C) Makefile $(SRC_H) expected.txt FileCheck.txt test3.bits new_file.bits
TEST_ACTUAL = actual.txt
EXECUTABLE = test_$(BASE_NAME)
EXECUTABLE_DBG = $(EXECUTABLE)_dbg
EXECUTABLE_GCOV = $(EXECUTABLE)_gcov
CFLAGS_GCOV =  $(CFLAGS) -fprofile-arcs -ftest-coverage
SHELL = /bin/bash 

$(EXECUTABLE): $(SRC_C) $(TEST_C) $(SRC_H) 
	$(CC) -o  $(EXECUTABLE) $(SRC_C) $(TEST_C) $(CFLAGS)

submit: $(SUBMIT_FILES)
	264submit $(ASG_NICKNAME) $(SUBMIT_FILES)

test: $(EXECUTABLE) $(TEST EXPECTED)
	@# If actual output matches expected output then count it as a success 
	@if diff -a -B <("./$(EXECUTABLE)") $(TEST_EXPECTED) &> /dev/null ; then \
		echo "Test passed: output of $(EXECUTABLE) matches $(TEST_EXPECTED)" ; \
	else \
		echo "Test failed: output of $(EXECUTABLE) does NOT match $(TEST_EXPECTED) " ; \
	fi
	valgrind ./$(EXECUTABLE)

pretest: 
	$(submit)
	264test $(ASG_NICKNAME)

clean:
	rm -f $(EXECUTABLE) $(EXECUTABLE_DBG) $(EXECUTABLE_GCOV) *.c.gcov *.gcda *.gcno

coverage:
	$(CC) -o $(EXECUTABLE_GCOV) $(SRC_C) $(TEST_C) $(CFLAGS_GCOV) -DNDEBUG
	./$(EXECUTABLE_GCOV)
	gcov -f $(SRC_C)

debug: 
	$(CC) -o $(EXECUTABLE_DBG) $(SRC_C) $(TEST_C) $(CFLAGS) -DEBUG
	./$(EXECUTABLE_DBG)

.PHONY: submit test pretest clean coverage debug 
