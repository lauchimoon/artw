CC = gcc
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
CFLAGS = -g -Wall -Wextra
OUT = artw

default: artw.o mdlexer.o dtree.o tag.o
	$(CC) -o $(OUT) $(CFLAGS) $(OBJ)

clean:
	rm *.o
	rm $(OUT)


artw.o: artw.c dtree.h tag.h
	$(CC) -c $<

dtree.o: dtree.c dtree.h tag.h
	$(CC) -c $<

mdlexer.o: mdlexer.c mdlexer.h
	$(CC) -c $<

tag.o: tag.c tag.h
	$(CC) -c $<
