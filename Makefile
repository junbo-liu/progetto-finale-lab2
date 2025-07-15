CC = gcc
CFLAGS = -Wall -pedantic -std=c11
NAME = main
OBJS = main.o parse_rescuers.o parse_env.o parse_emergency_types.o


all: $(NAME)
.PHONY: default clean

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# %.o: %.c
# 	$(CC) -c $(CFLAGS) $^ -o $@ 

$(NAME).o: $(NAME).c data_struct.c
	$(CC) -c $(CFLAGS) $< -o $@

parse_rescuers.o: parsers/parse_rescuers.c data_struct.c
	$(CC) -c $(CFLAGS) $< -o $@

parse_emergency_types.o: parsers/parse_emergency_types.c data_struct.c
	$(CC) -c $(CFLAGS) $< -o $@

parse_env.o: parsers/parse_env.c data_struct.c
	$(CC) -c $(CFLAGS) $< -o $@

run: $(NAME)
	./$(NAME)

clean:
	rm -f *.o $(NAME)
