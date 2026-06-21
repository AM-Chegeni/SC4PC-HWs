# Homework 03 Report – Matrix Multiplication

## Problem

We want to compute

\[
C = AB,
\qquad
C_{ij} = \sum_{k=0}^{N-1} A_{ik} B_{kj}.
\]

The matrices are special because every element of `A` is equal to `a`, and every element of `B` is equal to `b`:

\[
A = a \mathbf{1}_{N\times N},
\qquad
B = b \mathbf{1}_{N\times N}.
\]

Therefore,

\[
C_{ij}
=
\sum_{k=0}^{N-1} a b
=
Nab.
\]

So every element of `C` must be equal to `N*a*b`.

## How the code works

The program receives from the command line:

```bash
./matmul a b N fileout ordering
```

Example:

```bash
./matmul 2.0 3.0 4 C_N4.txt ijk
```

It then:

1. allocates matrices `A`, `B`, and `C`,
2. fills `A` with `a`,
3. fills `B` with `b`,
4. computes `C = A B`,
5. checks the result,
6. saves `C` to a file,
7. prints the execution time.

## Fast correctness check

The exact expected value is:

\[
C_{ij} = Nab.
\]

The simplest full check is to verify every element of `C`.

A faster idea is to check the sum:

\[
\sum_{i,j} C_{ij} = N^2(Nab).
\]

This is faster conceptually, but it is weaker than checking all elements because positive and negative errors could cancel out.

Therefore, the code prints both:

- an element-wise check,
- a checksum check.

## Floating-point tolerance

The code does not compare floating-point numbers using exact equality.

Instead of:

```c
C[i] == expected
```

it uses:

```c
fabs(C[i] - expected) < tolerance
```

This is necessary because decimal numbers are often not represented exactly in binary floating-point arithmetic.

## Loop ordering benchmark

The code implements six loop orderings:

- `ijk`
- `ikj`
- `jik`
- `jki`
- `kij`
- `kji`

All compute the same mathematical result, but they access memory differently.

In C, the matrix is stored in row-major order:

```c
M[i*N + j]
```

This means that elements with consecutive `j` are contiguous in memory.

Therefore, loop orderings where `j` is the innermost loop often perform better. In this code, `ikj` and `kij` are usually among the fastest.

## Why loop ordering matters

Modern CPUs are much faster when data are read from cache instead of main memory.

If memory access is contiguous, the CPU cache is used efficiently. If memory access jumps around, the program becomes slower.

This is why matrix multiplication is not only about the number of operations. It is also about how data are arranged and accessed in memory.

## Memory problems

The code stores three matrices:

\[
A, B, C.
\]

Each matrix has `N*N` elements. With `double`, each element uses 8 bytes.

So the approximate memory is:

\[
3N^2 \times 8 \ \text{bytes}.
\]

For example:

- `N = 1000`: about 24 MB
- `N = 5000`: about 600 MB
- `N = 10000`: about 2.4 GB

Large values of `N` can fail because the computer does not have enough RAM.

## Conclusion

The fastest loop ordering is usually the one with better memory locality. For C row-major arrays, accessing the `j` index in the innermost loop is usually better. Therefore, `ikj` or `kij` often run faster than the other orderings.
