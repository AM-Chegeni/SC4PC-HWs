/*
 * Homework 02 - Scientific Computing for Physics Students
 * Vector operation: d = a*x + y
 *
 * Usage:
 *   ./vector_sum N a x y
 *
 * Example:
 *   ./vector_sum 10 3 0.1 7.1
 *   ./vector_sum 1000000 3 0.1 7.1
 *   ./vector_sum 100000000 3 0.1 7.1
 */

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void print_usage(const char *progname) {
    printf("Usage: %s N a x y\n", progname);
    printf("Example: %s 1000000 3 0.1 7.1\n", progname);
}

static int parse_size_t(const char *s, size_t *out) {
    char *end = NULL;
    errno = 0;
    unsigned long long value = strtoull(s, &end, 10);

    if (errno != 0 || end == s || *end != '\0') {
        return 0;
    }

    *out = (size_t)value;
    return 1;
}

static int parse_double(const char *s, double *out) {
    char *end = NULL;
    errno = 0;
    double value = strtod(s, &end);

    if (errno != 0 || end == s || *end != '\0') {
        return 0;
    }

    *out = value;
    return 1;
}

static double elapsed_seconds(clock_t start, clock_t end) {
    return (double)(end - start) / CLOCKS_PER_SEC;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    size_t N;
    double a, x_value, y_value;

    if (!parse_size_t(argv[1], &N) || N == 0) {
        fprintf(stderr, "Error: N must be a positive integer.\n");
        return EXIT_FAILURE;
    }
    if (!parse_double(argv[2], &a) || !parse_double(argv[3], &x_value) || !parse_double(argv[4], &y_value)) {
        fprintf(stderr, "Error: a, x, and y must be valid floating-point numbers.\n");
        return EXIT_FAILURE;
    }

    printf("N = %zu\n", N);
    printf("a = %.17g, x = %.17g, y = %.17g\n", a, x_value, y_value);

    /*
     * Allocate vectors dynamically. For N = 1e8, three double vectors require
     * about 3 * 1e8 * 8 bytes = 2.4 GB of RAM.
     */
    double *x = malloc(N * sizeof(double));
    double *y = malloc(N * sizeof(double));
    double *d = malloc(N * sizeof(double));

    if (x == NULL || y == NULL || d == NULL) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        fprintf(stderr, "Requested memory for three double vectors: %.3f GB\n",
                (3.0 * (double)N * sizeof(double)) / 1e9);
        free(x);
        free(y);
        free(d);
        return EXIT_FAILURE;
    }

    clock_t t0 = clock();

    for (size_t i = 0; i < N; i++) {
        x[i] = x_value;
        y[i] = y_value;
    }

    for (size_t i = 0; i < N; i++) {
        d[i] = a * x[i] + y[i];
    }

    clock_t t1 = clock();

    double expected = a * x_value + y_value;

    /*
     * Floating-point numbers should not usually be compared with ==.
     * We use a tolerance because values such as 0.1 and 7.1 are not exactly
     * representable in binary floating point.
     */
    const double tolerance = 1e-12;
    size_t n_bad = 0;
    double max_abs_error = 0.0;

    for (size_t i = 0; i < N; i++) {
        double err = fabs(d[i] - expected);
        if (err > max_abs_error) {
            max_abs_error = err;
        }
        if (err > tolerance) {
            n_bad++;
        }
    }

    clock_t t2 = clock();

    printf("Expected a*x + y = %.17g\n", expected);
    printf("Expected printed with 20 decimals = %.20f\n", expected);
    printf("Mathematical value for a=3, x=0.1, y=7.1 is 7.4, but binary floating point may print 7.399999999999999...\n");
    printf("Maximum absolute error in d = %.17g\n", max_abs_error);
    printf("Number of elements failing tolerance %.1e: %zu\n", tolerance, n_bad);
    printf("Result: %s\n", (n_bad == 0) ? "PASS" : "FAIL");
    printf("Compute time: %.6f s\n", elapsed_seconds(t0, t1));
    printf("Check time: %.6f s\n", elapsed_seconds(t1, t2));
    printf("Approximate memory used by vectors: %.3f GB\n",
           (3.0 * (double)N * sizeof(double)) / 1e9);

    free(x);
    free(y);
    free(d);

    return (n_bad == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
