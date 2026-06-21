# Homework 07 - Random Numbers and Monte Carlo in C

This repository contains a C solution for Homework 07.

## Exercises

1. Coin tosses and the Law of Large Numbers
2. Monte Carlo estimate of pi
3. Change of variables: `Y = U^2`
4. Inverse transform sampling for the exponential distribution
5. Empirical CDF compared with the exact exponential CDF

The numerical calculations are implemented in C.  
Python is used only to plot the CSV files produced by the C program.

## Build

```bash
make
```

## Run

```bash
make run
```

This creates:

```text
data/coin_running_fraction.csv
data/pi_estimates.csv
data/y_u2_samples.csv
data/exponential_samples.csv
data/exponential_ecdf.csv
```

## Plot

```bash
pip install numpy matplotlib
make plots
```

Figures are saved in `plots/`.

## Clean

```bash
make clean
```
