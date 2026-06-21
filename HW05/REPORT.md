# Homework 05 — Stability of Legendre Polynomials

## Goal

For fixed order \(m=0\), we compute ordinary Legendre polynomials \(P_\ell(x)\) up to \(\ell_{max}=50\), for

\[
x = 0.1,\;0.5,\;0.9,\;0.99.
\]

The purpose is to compare the numerical behavior of the standard forward recurrence, a high-precision reference computation, and a Miller-style backward recurrence experiment.

## 1. Forward recurrence

The standard Bonnet recurrence is

\[
P_0(x)=1, \qquad P_1(x)=x,
\]

\[
P_{\ell+1}(x)=\frac{2\ell+1}{\ell+1}xP_\ell(x)-\frac{\ell}{\ell+1}P_{\ell-1}(x),
\qquad \ell\ge 1.
\]

This is implemented in double precision in `src/legendre_stability.c` and in `scripts/analyze_legendre.py`.

## 2. High-precision reference

The reference values are computed using `mpmath` with

```python
mp.mp.dps = 100
```

This means 100 decimal digits. These values are treated as exact for the purpose of measuring the double-precision errors.

## 3. Backward recurrence experiment

The recurrence can be rearranged as

\[
P_{\ell-1}(x)=\frac{2\ell+1}{\ell}xP_\ell(x)-\frac{\ell+1}{\ell}P_{\ell+1}(x).
\]

The Miller-style experiment starts from arbitrary values

\[
\widetilde P_{L+1}(x)=0, \qquad \widetilde P_L(x)=1,
\]

then propagates backward and rescales using

\[
c = \frac{1}{\widetilde P_0(x)},
\qquad
P_\ell^{back}(x)=c\widetilde P_\ell(x).
\]

In the code I use \(L=200\), while only the values up to \(\ell=50\) are compared.

## 4. Error calculation

For each method, I compute

\[
\Delta_{abs} = |P_\ell^{method}(x)-P_\ell^{ref}(x)|,
\]

and

\[
\Delta_{rel} = \frac{|P_\ell^{method}(x)-P_\ell^{ref}(x)|}{|P_\ell^{ref}(x)|}.
\]

When the reference value is zero or extremely close to zero, the relative error can become very large or less informative, so the absolute error is also inspected.

## 5. Numerical behavior

A three-term recurrence has two independent numerical solutions. The forward direction can amplify or suppress one of these solutions depending on the specific recurrence and region of parameters.

For ordinary Legendre polynomials \(P_\ell(x)\) on \(|x|<1\), the standard forward recurrence is numerically stable for the tested values. The double-precision values remain close to the high-precision reference values.

The backward recurrence experiment does not generally recover \(P_\ell(x)\). The reason is that Miller's algorithm is useful when the desired solution is the minimal solution in the direction of backward propagation. In this homework, using arbitrary final conditions and then rescaling with \(P_0=1\) does not select the ordinary Legendre polynomial solution cleanly.

## 6. Connection to spherical harmonics

For \(m=0\), spherical harmonics are related to Legendre polynomials by

\[
Y_{\ell 0}(\theta,\phi)=\sqrt{\frac{2\ell+1}{4\pi}}P_\ell(\cos\theta).
\]

Therefore, a numerical error in \(P_\ell(\cos\theta)\) directly propagates to \(Y_{\ell 0}\), multiplied only by the known normalization factor.

## Conclusion

The standard forward recurrence is the correct and stable method for this test. The arbitrary Miller-style backward recurrence generally fails to reproduce the ordinary Legendre polynomials for the tested values of \(x\).
