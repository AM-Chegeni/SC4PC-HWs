# Homework 07 Report

## 1. Coin tosses and LLN

We generate 100,000 fair coin tosses. Heads is represented by 1 and tails by 0.

The running fraction is

\[
f_N = \frac{1}{N}\sum_{i=1}^{N} X_i .
\]

For a fair coin, \(E[X]=0.5\). At small \(N\), the value fluctuates. As \(N\) grows, the fraction converges to 0.5, illustrating the Law of Large Numbers.

## 2. Monte Carlo estimate of \(\pi\)

We generate random points \((x,y)\) in the unit square. Points satisfying

\[
x^2+y^2 \le 1
\]

are inside the quarter circle. Therefore,

\[
\pi \approx 4\frac{N_{\rm inside}}{N}.
\]

The error generally decreases as \(1/\sqrt{N}\), with random fluctuations.

## 3. Change of variables: \(Y=U^2\)

If \(U\sim\mathrm{Uniform}(0,1)\) and \(Y=U^2\), then \(U=\sqrt{Y}\). Therefore,

\[
f_Y(y)=f_U(\sqrt{y})\left|\frac{d}{dy}\sqrt{y}\right|
      =\frac{1}{2\sqrt{y}},
\qquad 0<y<1.
\]

The histogram agrees with this analytic density.

## 4. Inverse transform exponential

For an exponential distribution,

\[
F(y)=1-e^{-\lambda y}.
\]

With \(U=F(y)\),

\[
Y=-\frac{\ln(1-U)}{\lambda}.
\]

We use \(\lambda=1.5\). The histogram is compared with

\[
f(y)=\lambda e^{-\lambda y}.
\]

## 5. Empirical CDF

After sorting the exponential sample,

\[
F_N(y_i)=\frac{i}{N}.
\]

The exact CDF is

\[
F(y)=1-e^{-\lambda y}.
\]

For large samples, the empirical CDF follows the exact CDF closely.
