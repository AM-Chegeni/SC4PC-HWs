#!/usr/bin/env python3
"""High-precision analysis for Homework 05.

Computes ordinary Legendre polynomials P_l(x) for x = 0.1, 0.5, 0.9, 0.99
using:
  - double precision forward recurrence
  - high precision reference recurrence with mpmath, mp.dps = 100
  - Miller-style backward recurrence experiment

Outputs:
  results/legendre_errors.csv
  results/relative_error_x_*.png
  results/absolute_error_x_*.png
"""

from __future__ import annotations

import csv
import math
from pathlib import Path

import matplotlib.pyplot as plt
import mpmath as mp

LMAX = 50
L_BACKWARD = 200
XS = [0.1, 0.5, 0.9, 0.99]
OUTDIR = Path("results")
OUTDIR.mkdir(exist_ok=True)
mp.mp.dps = 100


def forward_double(x: float, lmax: int) -> list[float]:
    p = [0.0] * (lmax + 1)
    p[0] = 1.0
    if lmax >= 1:
        p[1] = x
    for ell in range(1, lmax):
        p[ell + 1] = ((2 * ell + 1) / (ell + 1)) * x * p[ell] - (ell / (ell + 1)) * p[ell - 1]
    return p


def reference_mp(x: float, lmax: int) -> list[mp.mpf]:
    xx = mp.mpf(str(x))
    p = [mp.mpf("0")] * (lmax + 1)
    p[0] = mp.mpf("1")
    if lmax >= 1:
        p[1] = xx
    for ell in range(1, lmax):
        p[ell + 1] = (mp.mpf(2 * ell + 1) / (ell + 1)) * xx * p[ell] - (mp.mpf(ell) / (ell + 1)) * p[ell - 1]
    return p


def backward_double(x: float, lmax: int, L: int) -> list[float]:
    tmp = [0.0] * (L + 2)
    tmp[L + 1] = 0.0
    tmp[L] = 1.0
    for ell in range(L, 0, -1):
        tmp[ell - 1] = ((2 * ell + 1) / ell) * x * tmp[ell] - ((ell + 1) / ell) * tmp[ell + 1]
    c = 1.0 / tmp[0]
    return [c * tmp[ell] for ell in range(lmax + 1)]


def rel_error(value: float, ref: mp.mpf) -> float:
    abs_err = abs(mp.mpf(value) - ref)
    denom = abs(ref)
    if denom == 0:
        return float(abs_err)
    return float(abs_err / denom)


def main() -> None:
    rows = []
    for x in XS:
        pf = forward_double(x, LMAX)
        pb = backward_double(x, LMAX, L_BACKWARD)
        pref = reference_mp(x, LMAX)

        ell_values = list(range(LMAX + 1))
        rel_f = [rel_error(pf[ell], pref[ell]) for ell in ell_values]
        rel_b = [rel_error(pb[ell], pref[ell]) for ell in ell_values]
        abs_f = [float(abs(mp.mpf(pf[ell]) - pref[ell])) for ell in ell_values]
        abs_b = [float(abs(mp.mpf(pb[ell]) - pref[ell])) for ell in ell_values]

        for ell in ell_values:
            rows.append({
                "x": x,
                "ell": ell,
                "reference_mp": mp.nstr(pref[ell], 30),
                "forward_double": f"{pf[ell]:.17e}",
                "backward_double": f"{pb[ell]:.17e}",
                "abs_error_forward": f"{abs_f[ell]:.17e}",
                "rel_error_forward": f"{rel_f[ell]:.17e}",
                "abs_error_backward": f"{abs_b[ell]:.17e}",
                "rel_error_backward": f"{rel_b[ell]:.17e}",
            })

        label_x = str(x).replace(".", "p")
        plt.figure(figsize=(8, 5))
        plt.semilogy(ell_values, rel_f, marker="o", label="forward double")
        plt.semilogy(ell_values, rel_b, marker="s", label="backward experiment")
        plt.xlabel(r"degree $\ell$")
        plt.ylabel("relative error")
        plt.title(f"Relative error for x = {x}")
        plt.grid(True, which="both", alpha=0.3)
        plt.legend()
        plt.tight_layout()
        plt.savefig(OUTDIR / f"relative_error_x_{label_x}.png", dpi=160)
        plt.close()

        plt.figure(figsize=(8, 5))
        plt.semilogy(ell_values, abs_f, marker="o", label="forward double")
        plt.semilogy(ell_values, abs_b, marker="s", label="backward experiment")
        plt.xlabel(r"degree $\ell$")
        plt.ylabel("absolute error")
        plt.title(f"Absolute error for x = {x}")
        plt.grid(True, which="both", alpha=0.3)
        plt.legend()
        plt.tight_layout()
        plt.savefig(OUTDIR / f"absolute_error_x_{label_x}.png", dpi=160)
        plt.close()

    csv_path = OUTDIR / "legendre_errors.csv"
    with csv_path.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
        writer.writeheader()
        writer.writerows(rows)

    print(f"Wrote {csv_path}")
    print("Forward recurrence stays close to the high-precision reference.")
    print("The arbitrary backward/Miller-style experiment generally fails for ordinary P_l(x) on |x| < 1.")


if __name__ == "__main__":
    main()
