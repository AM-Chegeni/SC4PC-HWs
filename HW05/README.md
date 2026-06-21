# Homework 05 — Stability of Legendre Polynomials

This repository contains a solution for Homework 05 of the Scientific Computing for Physics Students course.

The homework studies the numerical stability of the ordinary Legendre polynomials \(P_\ell(x)\), for fixed order \(m=0\), degrees \(\ell = 0, \dots, 50\), and

\[
x = 0.1, 0.5, 0.9, 0.99.
\]

The main question is whether the usual forward three-term recurrence is stable, and whether an arbitrary Miller-style backward recurrence recovers the same polynomial values.

## Files

```text
src/legendre_stability.c        C implementation of forward and backward recurrences
scripts/analyze_legendre.py     Python high-precision reference, error analysis, and plots
Makefile                       build and run commands
REPORT.md                      written explanation and answers
results/                       generated CSV files and plots
```

## Build and run the C code

```bash
make
make run-c
```

This creates:

```text
results/legendre_c_output.csv
```

## Run the high-precision analysis

The Python script needs `mpmath` and `matplotlib`.

```bash
make run-python
```

or run everything:

```bash
make run
```

## Expected conclusion

For the tested values, the forward recurrence remains close to the high-precision reference. The arbitrary backward recurrence does not generally recover \(P_\ell(x)\), because Miller's algorithm works when the desired solution is minimal in the direction of the recurrence. Ordinary Legendre polynomials on \(|x|<1\) do not provide a clean separation that selects \(P_\ell\) in this experiment.
