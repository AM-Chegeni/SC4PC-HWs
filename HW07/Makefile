CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c11
LDFLAGS = -lm

TARGET = hw07
SRC = src/main.c src/rng.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

plots:
	python3 plots/plot_results.py

clean:
	rm -f $(TARGET)
	rm -f data/*.csv
	rm -f plots/*.png

.PHONY: all run plots clean
