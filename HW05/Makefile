CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c11
LDFLAGS = -lm

all: legendre_stability

legendre_stability: src/legendre_stability.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

run-c: legendre_stability
	mkdir -p results
	./legendre_stability 50 200 results/legendre_c_output.csv

run-python:
	python3 scripts/analyze_legendre.py

run: run-c run-python

clean:
	rm -f legendre_stability
	rm -f results/*.csv results/*.png
