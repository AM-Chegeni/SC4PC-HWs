CC = gcc
CFLAGS = -O3 -Wall -Wextra -std=c11
LDFLAGS = -lm

TARGET = matmul

all: $(TARGET)

$(TARGET): matmul.c
	$(CC) $(CFLAGS) matmul.c -o $(TARGET) $(LDFLAGS)

debug: matmul.c
	$(CC) -O0 -g -Wall -Wextra -std=c11 matmul.c -o $(TARGET)_debug $(LDFLAGS)

clean:
	rm -f $(TARGET) $(TARGET)_debug *.o C_*.txt benchmark_results.csv benchmark_plot.png
