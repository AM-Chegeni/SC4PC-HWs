# Homework 02 Report

## Goal

The goal of the homework is to write a C program that computes the vector operation

```text
d = a*x + y
```

where `a` is a scalar and `x`, `y`, and `d` are vectors with dimension `N`.

The required values of `N` are:

```text
N = 10, 10^6, 10^8
```

The values of `a`, `x`, and `y` are read from the command line.

## Implementation

The main code is `vector_sum.c`. It reads four command-line arguments:

```bash
./vector_sum N a x y
```

Example:

```bash
./vector_sum 10 3 0.1 7.1
```

The code allocates three vectors dynamically using `malloc`:

```c
double *x = malloc(N * sizeof(double));
double *y = malloc(N * sizeof(double));
double *d = malloc(N * sizeof(double));
```

Then it initializes the vectors and computes:

```c
d[i] = a * x[i] + y[i];
```

## Test of the result

Since all elements of `x` are equal and all elements of `y` are equal, all elements of `d` should be equal to

```text
expected = a*x + y
```

The code checks all elements of `d` and computes the maximum absolute error:

```c
fabs(d[i] - expected)
```

A tolerance is used:

```c
tolerance = 1e-12
```

This is better than using exact equality for floating-point numbers.

## Test case: a = 3, x = 0.1, y = 7.1

Mathematically:

```text
3 * 0.1 + 7.1 = 7.4
```

However, in a computer, numbers such as `0.1` and `7.1` cannot be represented exactly in binary floating point. Therefore, the result may be printed as a value extremely close to `7.4`, for example:

```text
7.399999999999999...
```

This means that a direct comparison such as

```c
result == 7.4
```

is not reliable. The correct approach is to compare with a tolerance:

```c
fabs(result - 7.4) < 1e-12
```

## Problem for large N

For `N = 10^8`, the full-vector code needs a large amount of memory.

The program stores three vectors:

```text
x, y, d
```

Each element is a `double`, which uses 8 bytes. Therefore, the required memory is approximately:

```text
3 * 10^8 * 8 bytes = 2.4 GB
```

This can be too large for some computers, especially if other programs are also using memory. The program may fail during memory allocation or become very slow.

For this reason, I also included `vector_sum_memory_light.c`, which performs the same numerical test without storing all vectors in memory.

## Conclusion

The homework shows two important points in scientific computing:

1. Large arrays can require a lot of memory, so memory usage must be estimated before running a code.
2. Floating-point numbers should usually be compared using a tolerance, not exact equality.
