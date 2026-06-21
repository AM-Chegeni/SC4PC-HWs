#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rng.h"

#define COIN_N 100000
#define SAMPLE_N 200000
#define LAMBDA 1.5
#define PI_VALUE 3.141592653589793238462643383279502884

static int compare_double(const void *a, const void *b)
{
    double da = *(const double *)a;
    double db = *(const double *)b;
    return (da > db) - (da < db);
}

void exercise1_coin_tosses(void)
{
    RNG rng;
    rng_seed(&rng, 12345);

    FILE *fp = fopen("data/coin_running_fraction.csv", "w");
    if (!fp) { perror("coin_running_fraction.csv"); exit(EXIT_FAILURE); }

    fprintf(fp, "n,running_fraction_heads\n");

    long heads = 0;

    for (long i = 0; i < COIN_N; i++) {
        int toss = (rng_uniform(&rng) < 0.5) ? 1 : 0;
        heads += toss;
        double fraction = (double)heads / (double)(i + 1);
        fprintf(fp, "%ld,%.17g\n", i + 1, fraction);
    }

    fclose(fp);

    printf("\nExercise 1 - Coin tosses and LLN\n");
    printf("Final running fraction of heads: %.8f\n", (double)heads / (double)COIN_N);
    printf("Expected value: 0.5\n");
    printf("Comment: the running fraction approaches 0.5 as N becomes large.\n");
}

void exercise2_monte_carlo_pi(void)
{
    RNG rng_x, rng_y;
    rng_seed(&rng_x, 111);
    rng_seed(&rng_y, 222);

    const long sample_sizes[] = {100, 1000, 10000, 100000, 1000000};
    const int n_sizes = 5;
    const long max_n = 1000000;

    FILE *fp = fopen("data/pi_estimates.csv", "w");
    if (!fp) { perror("pi_estimates.csv"); exit(EXIT_FAILURE); }

    fprintf(fp, "N,pi_estimate,absolute_error,one_over_sqrt_N\n");

    long inside = 0;
    int current_index = 0;

    printf("\nExercise 2 - Monte Carlo estimate of pi\n");
    printf("%-12s %-16s %-16s\n", "N", "pi_estimate", "abs_error");

    for (long i = 1; i <= max_n; i++) {
        double x = rng_uniform(&rng_x);
        double y = rng_uniform(&rng_y);

        if (x*x + y*y <= 1.0) inside++;

        if (current_index < n_sizes && i == sample_sizes[current_index]) {
            double pi_est = 4.0 * (double)inside / (double)i;
            double error = fabs(pi_est - PI_VALUE);
            double scaling = 1.0 / sqrt((double)i);

            fprintf(fp, "%ld,%.17g,%.17g,%.17g\n", i, pi_est, error, scaling);
            printf("%-12ld %-16.10f %-16.8e\n", i, pi_est, error);

            current_index++;
        }
    }

    fclose(fp);
    printf("Comment: the error decreases roughly like 1/sqrt(N), with random fluctuations.\n");
}

void exercise3_change_of_variables(void)
{
    RNG rng;
    rng_seed(&rng, 333);

    FILE *fp = fopen("data/y_u2_samples.csv", "w");
    if (!fp) { perror("y_u2_samples.csv"); exit(EXIT_FAILURE); }

    fprintf(fp, "y\n");

    double sum = 0.0;

    for (long i = 0; i < SAMPLE_N; i++) {
        double u = rng_uniform(&rng);
        double y = u*u;
        sum += y;
        fprintf(fp, "%.17g\n", y);
    }

    fclose(fp);

    printf("\nExercise 3 - Change of variables Y = U^2\n");
    printf("Sample mean: %.8f\n", sum / SAMPLE_N);
    printf("Expected mean: 1/3 = %.8f\n", 1.0/3.0);
    printf("Analytic density: f_Y(y) = 1/(2*sqrt(y)), for 0 < y < 1.\n");
}

double *exercise4_inverse_transform_exponential(void)
{
    RNG rng;
    rng_seed(&rng, 444);

    double *sample = malloc(SAMPLE_N * sizeof(double));
    if (!sample) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen("data/exponential_samples.csv", "w");
    if (!fp) { perror("exponential_samples.csv"); exit(EXIT_FAILURE); }

    fprintf(fp, "y\n");

    double sum = 0.0;

    for (long i = 0; i < SAMPLE_N; i++) {
        double u = rng_uniform(&rng);
        double y = -log(1.0 - u) / LAMBDA;
        sample[i] = y;
        sum += y;
        fprintf(fp, "%.17g\n", y);
    }

    fclose(fp);

    printf("\nExercise 4 - Inverse transform exponential\n");
    printf("lambda = %.3f\n", LAMBDA);
    printf("Sample mean: %.8f\n", sum / SAMPLE_N);
    printf("Expected mean: 1/lambda = %.8f\n", 1.0/LAMBDA);
    printf("Exact PDF: f(y) = lambda * exp(-lambda*y).\n");

    return sample;
}

void exercise5_empirical_cdf(double *sample)
{
    qsort(sample, SAMPLE_N, sizeof(double), compare_double);

    FILE *fp = fopen("data/exponential_ecdf.csv", "w");
    if (!fp) { perror("exponential_ecdf.csv"); exit(EXIT_FAILURE); }

    fprintf(fp, "y,empirical_cdf,exact_cdf\n");

    for (long i = 0; i < SAMPLE_N; i++) {
        double y = sample[i];
        double ecdf = (double)(i + 1) / (double)SAMPLE_N;
        double exact = 1.0 - exp(-LAMBDA * y);
        fprintf(fp, "%.17g,%.17g,%.17g\n", y, ecdf, exact);
    }

    fclose(fp);

    printf("\nExercise 5 - Empirical CDF\n");
    printf("Saved empirical CDF and exact CDF to data/exponential_ecdf.csv\n");
    printf("Exact CDF: F(y) = 1 - exp(-lambda*y).\n");
}

int main(void)
{
    exercise1_coin_tosses();
    exercise2_monte_carlo_pi();
    exercise3_change_of_variables();

    double *exponential_sample = exercise4_inverse_transform_exponential();
    exercise5_empirical_cdf(exponential_sample);

    free(exponential_sample);

    printf("\nAll CSV output files were saved in the data/ directory.\n");
    printf("Use: python3 plots/plot_results.py to generate plots.\n");

    return 0;
}
