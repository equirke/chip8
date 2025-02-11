CC=gcc
Objects= tokeniser.o
Flags= -g 
TEST_FLAGS = `pkg-config --cflags --libs check`
RAYLIB = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: run_tests

run_tests: parse_instruction_test processor_test
	./parse_instruction_test
	./processor_test

chip8_debug: main_debug.c processor.o parse_instruction.o processor.h chip8.h chip_constants.h
	$(CC) $(Flags) main_debug.c processor.o parse_instruction.o -o chip8_debug

chip8: main.c processor.o parse_instruction.o processor.h chip8.h chip_constants.h
	$(CC) $(Flags) main.c processor.o parse_instruction.o -o chip8 $(RAYLIB)
	
processor_test: processor.o processor_test.c processor.h
	$(CC) $(Flags) processor_test.c processor.o parse_instruction.o -o processor_test $(TEST_FLAGS)

parse_instruction_test: parse_instruction.o parse_instruction.h parse_instruction_test.c 
	$(CC) $(Flags) parse_instruction.o parse_instruction_test.c -o parse_instruction_test $(TEST_FLAGS)

processor.o: processor.c processor.h chip8.h chip_constants.h parse_instruction.h
	$(CC) $(Flags) -c processor.c
	
parse_instruction.o: parse_instruction.c chip_constants.h parse_instruction.h
	$(CC) $(Flags) -c parse_instruction.c