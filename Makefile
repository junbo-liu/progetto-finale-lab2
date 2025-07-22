CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -g
NAME = main

OBJS = main.o data_struct.o parse_rescuers.o parse_env.o parse_emergency_types.o

all: $(NAME)

main.o: main.c data_struct.h scall.h type.h parsers/parser.h
	$(CC) $(CFLAGS) -c $< -o $@

data_struct.o: data_struct.c data_struct.h scall.h type.h
	$(CC) $(CFLAGS) -c $< -o $@

parse_rescuers.o: parsers/parse_rescuers.c parsers/parser.h scall.h type.h data_struct.h
	$(CC) $(CFLAGS) -c $< -o $@

parse_env.o: parsers/parse_env.c parsers/parser.h scall.h type.h data_struct.h
	$(CC) $(CFLAGS) -c $< -o $@

parse_emergency_types.o: parsers/parse_emergency_types.c parsers/parser.h scall.h type.h data_struct.h
	$(CC) $(CFLAGS) -c $< -o $@

client.o: client.c scall.h data_struct.h parsers/parser.h
	$(CC) $(CFLAGS) -c $< -o $@

client: client.o parse_env.o
	$(CC) $(CFLAGS) -o $@ $^ 

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


run: $(NAME)
	./$(NAME)

clean:
	rm -f *.o $(NAME)
