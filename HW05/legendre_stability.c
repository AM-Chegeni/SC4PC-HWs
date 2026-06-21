#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 Homework 05: Stability of Legendre Polynomials

 This C program computes ordinary Legendre polynomials P_l(x) using:
   1. standard forward Bonnet recurrence in double precision
   2. Miller-style backward recurrence experiment with rescaling P_0 = 1

 It writes one CSV file with columns:
   x,l,forward,backward,abs_diff_forward_backward

 The high-precision reference and plots are produced by scripts/analyze_legendre.py.
*/

static void legendre_forward(double x, int lmax, double *p) {
    p[0] = 1.0;
    if (lmax >= 1) p[1] = x;

    for (int l = 1; l < lmax; ++l) {
        p[l + 1] = ((2.0 * l + 1.0) / (l + 1.0)) * x * p[l]
                 - (l / (l + 1.0)) * p[l - 1];
    }
}

static void legendre_backward(double x, int lmax, int L, double *pback) {
    /* Need temporary values from 0 to L+1. */
    double *tmp = calloc((size_t)L + 2, sizeof(double));
    if (!tmp) {
        fprintf(stderr, "Allocation failed in backward recurrence.\n");
        exit(EXIT_FAILURE);
    }

    tmp[L + 1] = 0.0;
    tmp[L] = 1.0;

    for (int l = L; l >= 1; --l) {
        tmp[l - 1] = ((2.0 * l + 1.0) / l) * x * tmp[l]
                   - ((l + 1.0) / l) * tmp[l + 1];
    }

    if (tmp[0] == 0.0 || !isfinite(tmp[0])) {
        for (int l = 0; l <= lmax; ++l) pback[l] = NAN;
    } else {
        double c = 1.0 / tmp[0];
        for (int l = 0; l <= lmax; ++l) pback[l] = c * tmp[l];
    }

    free(tmp);
}

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s LMAX L_BACKWARD output.csv\n", prog);
    fprintf(stderr, "Example: %s 50 200 results/legendre_c_output.csv\n", prog);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    int lmax = atoi(argv[1]);
    int L = atoi(argv[2]);
    const char *fileout = argv[3];

    if (lmax < 1 || L < lmax) {
        fprintf(stderr, "Error: require LMAX >= 1 and L_BACKWARD >= LMAX.\n");
        return EXIT_FAILURE;
    }

    const double xs[] = {0.1, 0.5, 0.9, 0.99};
    const int nx = (int)(sizeof(xs) / sizeof(xs[0]));

    double *pf = malloc(((size_t)lmax + 1) * sizeof(double));
    double *pb = malloc(((size_t)lmax + 1) * sizeof(double));
    if (!pf || !pb) {
        fprintf(stderr, "Allocation failed.\n");
        free(pf);
        free(pb);
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(fileout, "w");
    if (!fp) {
        perror("Could not open output file");
        free(pf);
        free(pb);
        return EXIT_FAILURE;
    }

    fprintf(fp, "x,l,forward,backward,abs_diff_forward_backward\n");

    for (int ix = 0; ix < nx; ++ix) {
        double x = xs[ix];
        legendre_forward(x, lmax, pf);
        legendre_backward(x, lmax, L, pb);

        for (int l = 0; l <= lmax; ++l) {
            double diff = fabs(pf[l] - pb[l]);
            fprintf(fp, "%.17g,%d,%.17e,%.17e,%.17e\n", x, l, pf[l], pb[l], diff);
        }
    }

    fclose(fp);
    free(pf);
    free(pb);

    printf("Wrote %s\n", fileout);
    return EXIT_SUCCESS;
}
