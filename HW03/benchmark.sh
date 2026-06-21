#!/usr/bin/env bash
set -e

make

A=2.0
B=3.0

# Keep sizes moderate for normal laptops.
# You can increase them if your computer has enough memory and time.
SIZES=(100 200 400 600)
ORDERS=(ijk ikj jik jki kij kji)

OUTCSV="benchmark_results.csv"

echo "N,ordering,time_seconds" > "$OUTCSV"

for N in "${SIZES[@]}"; do
    for ORDER in "${ORDERS[@]}"; do
        OUTFILE="C_${N}_${ORDER}.txt"

        echo "Running N=$N ordering=$ORDER"

        OUTPUT=$(./matmul "$A" "$B" "$N" "$OUTFILE" "$ORDER")

        TIME=$(echo "$OUTPUT" | awk '/Elapsed time/ {print $4}')

        echo "$N,$ORDER,$TIME" >> "$OUTCSV"

        # To avoid filling the disk during benchmarks:
        rm -f "$OUTFILE"
    done
done

echo "Benchmark saved to $OUTCSV"
