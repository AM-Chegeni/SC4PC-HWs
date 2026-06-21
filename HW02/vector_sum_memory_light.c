/*
 * Memory-light version of Homework 02.
 * It computes d_i = a*x_i + y_i without storing full x, y, and d vectors.
 * This is useful when N is very large, e.g. N = 1e8.
 *
 * Usage:
 *   ./vector_sum_memory_light N a x y
 */

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int parse_size_t(const char *s, size_t *out) {
    char *end = NULL;
    errno = 0;
    unsigned long long value = strtoull(s, &end, 10);
    if (errno != 0 || end == s || *end != '\0') return 0;
    *out = (size_t)value;
    return 1;
}

static int parse_double(const char *s, double *out) {
    char *end = NULL;
    errno = 0;
    double value = strtod(s, &end);
    if (errno != 0 || end == s || *end != '\0') return 0;
    *out = value;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s N a x y\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t N;
    double a, x_value, y_value;
    if (!parse_size_t(argv[1], &N) || N == 0 ||
        !parse_double(argv[2], &a) ||
        !parse_double(argv[3], &x_value) ||
        !parse_double(argv[4], &y_value)) {
        fprintf(stderr, "Invalid input. Example: %s 100000000 3 0.1 7.1\n", argv[0]);
        return EXIT_FAILURE;
    }

    double expected = a * x_value + y_value;
    const double tolerance = 1e-12;
    size_t n_bad = 0;
    double max_abs_error = 0.0;

    clock_t t0 = clock();
    for (size_t i = 0; i < N; i++) {
        double d_i = a * x_value + y_value;
        double err = fabs(d_i - expected);
        if (err > max_abs_error) max_abs_error = err;
        if (err > tolerance) n_bad++;
    }
    clock_t t1 = clock();

    printf("N = %zu\n", N);
    printf("a = %.17g, x = %.17g, y = %.17g\n", a, x_value, y_value);
    printf("Expected a*x + y = %.17g\n", expected);
    printf("Expected printed with 20 decimals = %.20f\n", expected);
    printf("Maximum absolute error in d = %.17g\n", max_abs_error);
    printf("Number of elements failing tolerance %.1e: %zu\n", tolerance, n_bad);
    printf("Result: %s\n", (n_bad == 0) ? "PASS" : "FAIL");
    printf("Elapsed CPU time: %.6f s\n", (double)(t1 - t0) / CLOCKS_PER_SEC);
    printf("Approximate stored vector memory: almost zero, because vectors are not allocated.\n");

    return (n_bad == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
