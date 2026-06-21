CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c11
LDFLAGS = -lm

all: vector_sum vector_sum_memory_light floating_point_test

vector_sum: vector_sum.c
	$(CC) $(CFLAGS) -o vector_sum vector_sum.c $(LDFLAGS)

vector_sum_memory_light: vector_sum_memory_light.c
	$(CC) $(CFLAGS) -o vector_sum_memory_light vector_sum_memory_light.c $(LDFLAGS)

floating_point_test: floating_point_test.c
	$(CC) $(CFLAGS) -o floating_point_test floating_point_test.c $(LDFLAGS)

run-small: all
	./vector_sum 10 3 0.1 7.1
	./floating_point_test

run-medium: all
	./vector_sum 1000000 3 0.1 7.1

run-large-light: all
	./vector_sum_memory_light 100000000 3 0.1 7.1

clean:
	rm -f vector_sum vector_sum_memory_light floating_point_test
