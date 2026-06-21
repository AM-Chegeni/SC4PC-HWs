# Homework 03 – Matrix Multiplication

This repository contains a C solution for **Scientific Computing for Physics Students – Homework 03**.

The task is to compute the matrix multiplication

\[
C = AB, \qquad C_{ij} = \sum_{k=0}^{N-1} A_{ik} B_{kj}
\]

with constant matrices

\[
A = a \mathbf{1}_{N\times N}, \qquad B = b \mathbf{1}_{N\times N}.
\]

Because every element of `A` is `a` and every element of `B` is `b`, the expected result is

\[
C_{ij} = N a b.
\]

So the quickest correctness check is not to compare every element manually. We can compute the expected value once and test the matrix using either:

1. a full tolerance check over all elements, or
2. a faster checksum/sum check.

## Files

| File | Description |
|---|---|
| `matmul.c` | Main program. Computes matrix multiplication using selectable loop ordering. |
| `Makefile` | Compile and clean commands. |
| `benchmark.sh` | Runs benchmarks for different loop orderings and matrix sizes. |
| `report.md` | Short written explanation for the homework. |
| `sample_output.txt` | Example output from running the program. |
| `benchmark_results.csv` | Example benchmark output generated on this machine. |
| `plot_benchmark.py` | Optional Python script to plot benchmark results. |

## Compilation

```bash
make
```

This creates the executable:

```bash
./matmul
```

## Usage

```bash
./matmul a b N fileout ordering
```

Example:

```bash
./matmul 2.0 3.0 4 C_N4.txt ijk
```

Arguments:

- `a`: value of every element in matrix A
- `b`: value of every element in matrix B
- `N`: matrix size
- `fileout`: output filename for matrix C
- `ordering`: loop ordering, one of:
  - `ijk`
  - `ikj`
  - `jik`
  - `jki`
  - `kij`
  - `kji`

## Example

```bash
./matmul 2.0 3.0 4 C_N4.txt ijk
```

Expected value:

\[
C_{ij} = N a b = 4 \times 2 \times 3 = 24.
\]

So every element of C should be `24`.

## Benchmark

Run:

```bash
./benchmark.sh
```

This writes timings to:

```bash
benchmark_results.csv
```

## Important note about speed

In C, matrices stored as a 1D array in row-major order have contiguous elements along the last index:

```c
M[i*N + j]
```

So accessing `j` in the innermost loop is usually faster because memory is accessed contiguously.

For this reason, loop orders like `ikj` and `kij` are usually faster than `ijk`, `jik`, `jki`, or `kji`.

## Important note about memory

The program stores three `N x N` matrices: `A`, `B`, and `C`.

Using `double`, memory is approximately:

\[
3 N^2 \times 8 \ \text{bytes}.
\]

Examples:

| N | Approx memory |
|---|---|
| 1000 | 24 MB |
| 5000 | 600 MB |
| 10000 | 2.4 GB |

Large values of `N` can fail because of insufficient RAM.
