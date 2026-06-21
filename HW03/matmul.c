/*
Homework 03 - Matrix Multiplication

Compute C = A B, where:
    A = a * 1_N
    B = b * 1_N

Therefore every element of C should be:
    C_ij = N * a * b

Usage:
    ./matmul a b N fileout ordering

Example:
    ./matmul 2.0 3.0 4 C_N4.txt ijk

Loop orderings:
    ijk, ikj, jik, jki, kij, kji
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <errno.h>

#define IDX(i, j, N) ((i) * (N) + (j))

static double wall_time_seconds(void) {
    struct timespec ts;

#if defined(CLOCK_MONOTONIC)
    clock_gettime(CLOCK_MONOTONIC, &ts);
#else
    timespec_get(&ts, TIME_UTC);
#endif

    return (double)ts.tv_sec + (double)ts.tv_nsec * 1.0e-9;
}

static void print_usage(const char *prog) {
    fprintf(stderr, "Usage: %s a b N fileout ordering\n", prog);
    fprintf(stderr, "Example: %s 2.0 3.0 4 C_N4.txt ijk\n", prog);
    fprintf(stderr, "ordering must be one of: ijk, ikj, jik, jki, kij, kji\n");
}

static int parse_double(const char *s, double *value) {
    char *endptr = NULL;
    errno = 0;
    *value = strtod(s, &endptr);

    if (errno != 0 || endptr == s || *endptr != '\0') {
        return 0;
    }
    return 1;
}

static int parse_size(const char *s, size_t *value) {
    char *endptr = NULL;
    errno = 0;
    unsigned long long tmp = strtoull(s, &endptr, 10);

    if (errno != 0 || endptr == s || *endptr != '\0' || tmp == 0) {
        return 0;
    }

    *value = (size_t)tmp;
    return 1;
}

static void fill_matrix(double *M, size_t N, double value) {
    size_t total = N * N;
    for (size_t i = 0; i < total; i++) {
        M[i] = value;
    }
}

static void zero_matrix(double *M, size_t N) {
    size_t total = N * N;
    for (size_t i = 0; i < total; i++) {
        M[i] = 0.0;
    }
}

/*
The following functions compute exactly the same mathematical operation,
but with different loop orderings.

The matrix is stored in row-major order:
    M[i][j] -> M[i*N + j]

In row-major storage, increasing j accesses contiguous memory.
This is usually faster because it uses CPU cache more efficiently.
*/

static void matmul_ijk(const double *A, const double *B, double *C, size_t N) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < N; k++) {
                sum += A[IDX(i, k, N)] * B[IDX(k, j, N)];
            }
            C[IDX(i, j, N)] = sum;
        }
    }
}

static void matmul_ikj(const double *A, const double *B, double *C, size_t N) {
    zero_matrix(C, N);

    for (size_t i = 0; i < N; i++) {
        for (size_t k = 0; k < N; k++) {
            double aik = A[IDX(i, k, N)];
            for (size_t j = 0; j < N; j++) {
                C[IDX(i, j, N)] += aik * B[IDX(k, j, N)];
            }
        }
    }
}

static void matmul_jik(const double *A, const double *B, double *C, size_t N) {
    for (size_t j = 0; j < N; j++) {
        for (size_t i = 0; i < N; i++) {
            double sum = 0.0;
            for (size_t k = 0; k < N; k++) {
                sum += A[IDX(i, k, N)] * B[IDX(k, j, N)];
            }
            C[IDX(i, j, N)] = sum;
        }
    }
}

static void matmul_jki(const double *A, const double *B, double *C, size_t N) {
    zero_matrix(C, N);

    for (size_t j = 0; j < N; j++) {
        for (size_t k = 0; k < N; k++) {
            double bkj = B[IDX(k, j, N)];
            for (size_t i = 0; i < N; i++) {
                C[IDX(i, j, N)] += A[IDX(i, k, N)] * bkj;
            }
        }
    }
}

static void matmul_kij(const double *A, const double *B, double *C, size_t N) {
    zero_matrix(C, N);

    for (size_t k = 0; k < N; k++) {
        for (size_t i = 0; i < N; i++) {
            double aik = A[IDX(i, k, N)];
            for (size_t j = 0; j < N; j++) {
                C[IDX(i, j, N)] += aik * B[IDX(k, j, N)];
            }
        }
    }
}

static void matmul_kji(const double *A, const double *B, double *C, size_t N) {
    zero_matrix(C, N);

    for (size_t k = 0; k < N; k++) {
        for (size_t j = 0; j < N; j++) {
            double bkj = B[IDX(k, j, N)];
            for (size_t i = 0; i < N; i++) {
                C[IDX(i, j, N)] += A[IDX(i, k, N)] * bkj;
            }
        }
    }
}

static int check_all_elements(const double *C, size_t N, double expected, double tolerance) {
    size_t total = N * N;

    for (size_t i = 0; i < total; i++) {
        if (fabs(C[i] - expected) > tolerance) {
            fprintf(stderr,
                    "Check failed at flat index %zu: C = %.17g, expected = %.17g, diff = %.17g\n",
                    i, C[i], expected, fabs(C[i] - expected));
            return 0;
        }
    }

    return 1;
}

/*
Fast check:
If all values should be expected_value, then the sum of all matrix elements
should be:
    expected_sum = N*N*expected_value

This is much faster to write as an idea, but it is weaker than checking
every element because different errors could cancel in the sum.
*/
static int check_sum(const double *C, size_t N, double expected_value, double tolerance) {
    size_t total = N * N;
    double sum = 0.0;

    for (size_t i = 0; i < total; i++) {
        sum += C[i];
    }

    double expected_sum = (double)total * expected_value;
    double diff = fabs(sum - expected_sum);

    printf("Checksum: sum(C) = %.17g\n", sum);
    printf("Expected sum     = %.17g\n", expected_sum);
    printf("Checksum diff    = %.17g\n", diff);

    return diff <= tolerance * (double)total;
}

static int save_matrix(const char *filename, const double *C, size_t N) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Could not open output file");
        return 0;
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            fprintf(fp, "%.17g", C[IDX(i, j, N)]);
            if (j + 1 < N) {
                fprintf(fp, " ");
            }
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    return 1;
}

int main(int argc, char **argv) {
    if (argc != 6) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    double a, b;
    size_t N;
    const char *fileout = argv[4];
    const char *ordering = argv[5];

    if (!parse_double(argv[1], &a)) {
        fprintf(stderr, "Error: invalid value for a: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (!parse_double(argv[2], &b)) {
        fprintf(stderr, "Error: invalid value for b: %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    if (!parse_size(argv[3], &N)) {
        fprintf(stderr, "Error: invalid value for N: %s\n", argv[3]);
        return EXIT_FAILURE;
    }

    if (N > 0 && N > ((size_t)-1) / N) {
        fprintf(stderr, "Error: N*N overflows size_t.\n");
        return EXIT_FAILURE;
    }

    size_t total = N * N;

    if (total > ((size_t)-1) / sizeof(double)) {
        fprintf(stderr, "Error: matrix size is too large.\n");
        return EXIT_FAILURE;
    }

    double memory_gb = 3.0 * (double)total * (double)sizeof(double) / 1.0e9;
    printf("N = %zu\n", N);
    printf("Approximate memory for A, B, C = %.3f GB\n", memory_gb);

    double *A = malloc(total * sizeof(double));
    double *B = malloc(total * sizeof(double));
    double *C = malloc(total * sizeof(double));

    if (A == NULL || B == NULL || C == NULL) {
        fprintf(stderr, "Error: memory allocation failed. Try a smaller N.\n");
        free(A);
        free(B);
        free(C);
        return EXIT_FAILURE;
    }

    fill_matrix(A, N, a);
    fill_matrix(B, N, b);

    double t0 = wall_time_seconds();

    if (strcmp(ordering, "ijk") == 0) {
        matmul_ijk(A, B, C, N);
    } else if (strcmp(ordering, "ikj") == 0) {
        matmul_ikj(A, B, C, N);
    } else if (strcmp(ordering, "jik") == 0) {
        matmul_jik(A, B, C, N);
    } else if (strcmp(ordering, "jki") == 0) {
        matmul_jki(A, B, C, N);
    } else if (strcmp(ordering, "kij") == 0) {
        matmul_kij(A, B, C, N);
    } else if (strcmp(ordering, "kji") == 0) {
        matmul_kji(A, B, C, N);
    } else {
        fprintf(stderr, "Error: unknown ordering: %s\n", ordering);
        print_usage(argv[0]);
        free(A);
        free(B);
        free(C);
        return EXIT_FAILURE;
    }

    double t1 = wall_time_seconds();
    double elapsed = t1 - t0;

    double expected = (double)N * a * b;

    /*
    Tolerance for floating-point arithmetic.
    The exact result may not be represented exactly in binary.
    */
    double tolerance = 1.0e-10 * fmax(1.0, fabs(expected));

    printf("a = %.17g\n", a);
    printf("b = %.17g\n", b);
    printf("Expected C_ij = N*a*b = %.17g\n", expected);
    printf("Loop ordering = %s\n", ordering);
    printf("Elapsed time  = %.9f seconds\n", elapsed);

    int ok_full = check_all_elements(C, N, expected, tolerance);
    int ok_sum = check_sum(C, N, expected, tolerance);

    printf("Full element-wise check: %s\n", ok_full ? "PASS" : "FAIL");
    printf("Fast checksum check:     %s\n", ok_sum ? "PASS" : "FAIL");

    if (!save_matrix(fileout, C, N)) {
        free(A);
        free(B);
        free(C);
        return EXIT_FAILURE;
    }

    printf("Matrix C saved to: %s\n", fileout);

    free(A);
    free(B);
    free(C);

    return (ok_full && ok_sum) ? EXIT_SUCCESS : EXIT_FAILURE;
}
