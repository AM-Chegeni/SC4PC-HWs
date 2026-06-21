/*
 * Small test showing the floating-point issue in point 4 of the homework.
 */

#include <math.h>
#include <stdio.h>

int main(void) {
    double a = 3.0;
    double x = 0.1;
    double y = 7.1;
    double result = a * x + y;

    printf("a*x + y with 17 significant digits: %.17g\n", result);
    printf("a*x + y with 20 decimal digits: %.20f\n", result);
    printf("Direct comparison with 7.4: %s\n", (result == 7.4) ? "true" : "false");
    printf("Tolerance comparison |result - 7.4| < 1e-12: %s\n",
           (fabs(result - 7.4) < 1e-12) ? "true" : "false");

    return 0;
}
