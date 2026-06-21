# Homework 02 — Vector Sum in C

This repository contains a solution for Homework 02 of the Scientific Computing for Physics Students course.

The homework asks for a C program that computes

```text
d = a*x + y
```

where `a` is a scalar and `d`, `x`, and `y` are vectors of dimension `N`.

The tested values are:

```text
a = 3
x = 0.1
y = 7.1
```

Mathematically, the result is:

```text
3 * 0.1 + 7.1 = 7.4
```

## Files

- `vector_sum.c`: main solution using full dynamically allocated vectors `x`, `y`, and `d`.
- `vector_sum_memory_light.c`: memory-light version for very large `N`, especially `N = 10^8`.
- `floating_point_test.c`: small program showing why direct comparison with `7.4` can fail.
- `Makefile`: build and run commands.
- `report.md`: short explanation of the homework results.
- `.gitignore`: ignores compiled files and temporary outputs.

## Compile

```bash
make
```

or manually:

```bash
gcc -O2 -Wall -Wextra -std=c11 -o vector_sum vector_sum.c -lm
gcc -O2 -Wall -Wextra -std=c11 -o vector_sum_memory_light vector_sum_memory_light.c -lm
gcc -O2 -Wall -Wextra -std=c11 -o floating_point_test floating_point_test.c -lm
```

## Run

For `N = 10`:

```bash
./vector_sum 10 3 0.1 7.1
```

For `N = 10^6`:

```bash
./vector_sum 1000000 3 0.1 7.1
```

For `N = 10^8`:

```bash
./vector_sum 100000000 3 0.1 7.1
```

If the full-vector version fails for `N = 10^8`, use:

```bash
./vector_sum_memory_light 100000000 3 0.1 7.1
```

## Why can `N = 10^8` be a problem?

The full-vector program stores three vectors of type `double`:

```text
x, y, d
```

Each `double` uses 8 bytes. Therefore, for `N = 10^8`, the required memory is approximately:

```text
3 * 10^8 * 8 bytes = 2.4 GB
```

This can fail on computers with limited available RAM.

## Floating-point issue

The value `0.1` is not exactly representable in binary floating point. Therefore, the calculation

```text
3 * 0.1 + 7.1
```

may be stored as a number very close to `7.4`, but not exactly equal to it.

For this reason, the code uses a tolerance test:

```c
fabs(d[i] - expected) < 1e-12
```

instead of checking:

```c
d[i] == 7.4
```
