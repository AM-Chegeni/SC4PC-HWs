import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DATA = ROOT / "data"
PLOTS = ROOT / "plots"

coin = np.genfromtxt(DATA / "coin_running_fraction.csv", delimiter=",", names=True)
plt.figure(figsize=(8, 5))
plt.plot(coin["n"], coin["running_fraction_heads"], label="Running fraction")
plt.axhline(0.5, linestyle="--", label="Expected value = 0.5")
plt.xlabel("Number of tosses")
plt.ylabel("Fraction of heads")
plt.title("Law of Large Numbers for fair coin tosses")
plt.legend()
plt.tight_layout()
plt.savefig(PLOTS / "coin_running_fraction.png", dpi=150)
plt.close()

pi_data = np.genfromtxt(DATA / "pi_estimates.csv", delimiter=",", names=True)
plt.figure(figsize=(8, 5))
plt.loglog(pi_data["N"], pi_data["absolute_error"], marker="o", label="Monte Carlo error")
plt.loglog(pi_data["N"], pi_data["one_over_sqrt_N"], linestyle="--", label=r"$1/\sqrt{N}$")
plt.xlabel("N")
plt.ylabel(r"$|\hat{\pi}-\pi|$")
plt.title("Monte Carlo estimate of pi")
plt.legend()
plt.tight_layout()
plt.savefig(PLOTS / "pi_error.png", dpi=150)
plt.close()

y_u2 = np.genfromtxt(DATA / "y_u2_samples.csv", delimiter=",", names=True)["y"]
grid = np.linspace(1e-4, 1.0, 1000)
pdf = 1.0 / (2.0 * np.sqrt(grid))
plt.figure(figsize=(8, 5))
plt.hist(y_u2, bins=100, density=True, alpha=0.6, label=r"Histogram of $Y=U^2$")
plt.plot(grid, pdf, label=r"Analytic PDF $1/(2\sqrt{y})$")
plt.xlabel("y")
plt.ylabel("Density")
plt.title(r"Change of variables: $Y=U^2$")
plt.legend()
plt.tight_layout()
plt.savefig(PLOTS / "change_of_variables_y_u2.png", dpi=150)
plt.close()

exp_sample = np.genfromtxt(DATA / "exponential_samples.csv", delimiter=",", names=True)["y"]
lam = 1.5
grid = np.linspace(0.0, np.percentile(exp_sample, 99.5), 1000)
pdf = lam * np.exp(-lam * grid)
plt.figure(figsize=(8, 5))
plt.hist(exp_sample, bins=120, density=True, alpha=0.6, label="Generated sample")
plt.plot(grid, pdf, label=r"Exact PDF $\lambda e^{-\lambda y}$")
plt.xlabel("y")
plt.ylabel("Density")
plt.title("Exponential distribution from inverse transform")
plt.legend()
plt.tight_layout()
plt.savefig(PLOTS / "exponential_pdf.png", dpi=150)
plt.close()

ecdf = np.genfromtxt(DATA / "exponential_ecdf.csv", delimiter=",", names=True)
step = max(1, len(ecdf) // 5000)
plt.figure(figsize=(8, 5))
plt.plot(ecdf["y"][::step], ecdf["empirical_cdf"][::step], ".", markersize=2, label="Empirical CDF")
plt.plot(ecdf["y"][::step], ecdf["exact_cdf"][::step], label="Exact CDF")
plt.xlabel("y")
plt.ylabel("F(y)")
plt.title("Empirical CDF of exponential sample")
plt.legend()
plt.tight_layout()
plt.savefig(PLOTS / "exponential_ecdf.png", dpi=150)
plt.close()

print("Plots saved in:", PLOTS)
