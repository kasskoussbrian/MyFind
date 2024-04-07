CC=gcc
CFLAGS=-std=c99  -pedantic  -Werror -Wall -Wextra -Wvla  
LDFLAGS= 
LDLIBS=

OBJ=src/main.o src/basic_find/simple.o src/basic_find/errcheck.o src/token/expr.o src/token/starting.o src/token/parse_arg.o src/token/stack.o src/basic_find/evaluate.o


myfind:$(OBJ)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJ) -o myfind

all:myfind

debug: CFLAGS+= -fsanitize=address -g
debug: LDFLAGS+= -fsanitize=address -g
debug : all



	

clean:
	rm src/basic_find/*.o src/token/*.o src/*.o   myfind
