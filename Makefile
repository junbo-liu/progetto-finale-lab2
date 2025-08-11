CC = gcc
CFLAGS = -Wall -pedantic -std=c11 

NAME = main

WITH_FILE = -f request.txt
ARGS = arg1 arg2 arg3 arg4

OBJS = main.o data_struct.o parse_rescuers.o parse_env.o parse_emergency_types.o file_log.o thrd_codificatore.o coda_emergenze.o



main.o: main.c data_struct.h macro.h type.h parser.h  file_log.h thrd_codificatore.h

data_struct.o: data_struct.c data_struct.h macro.h type.h

parse_rescuers.o: parse_rescuers.c parser.h macro.h type.h data_struct.h

parse_env.o: parse_env.c macro.h data_struct.h parser.h type.h file_log.h

parse_emergency_types.o: parse_emergency_types.c parser.h macro.h type.h data_struct.h

file_log.o: file_log.c file_log.h macro.h

thrd_codificatore.o: thrd_codificatore.c thrd_codificatore.h macro.h coda_emergenze.h file_log.h

coda_emergenze.o: coda_emergenze.c coda_emergenze.h type.h macro.h

client.o: client.c macro.h type.h data_struct.h

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ 

all: $(NAME) client

.PHONY: all run runc clean maindebug

client: client.o data_struct.o
	$(CC) $(CFLAGS) -o $@ $^ 

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

runc: client
	./client $(WITH_FILE)

maindebug: main
	valgrind --leak-check=full ./main
	
run: $(NAME)
	./$(NAME)

clean:
	rm -f *.o $(NAME)
