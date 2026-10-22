/*
 * A383843  Square array A(n,k), n>=0, k>=0, read by antidiagonals downwards,
 *          where column k is the expansion of 1/Product_{j=0..k} (1 - j*x)^2.
 * https://oeis.org/A383843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) read by antidiagonals of the square array A(n,k) with *
 *            A(n,k) = Σ_{t=0..n} Stirling2(t+k,k) * Stirling2(n-t+k,k)
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_STIRLING 300

static seq_t stirling[MAX_STIRLING + 1][MAX_STIRLING + 1];
static int stirling_initialized = 0;

static void init_stirling(void) {
    if (stirling_initialized) return;
    for (int n = 0; n <= MAX_STIRLING; ++n)
        for (int k = 0; k <= MAX_STIRLING; ++k)
            stirling[n][k] = 0;
    stirling[0][0] = 1;
    for (int n = 1; n <= MAX_STIRLING; ++n) {
        for (int k = 1; k <= n; ++k) {
            stirling[n][k] = (stirling[n-1][k] * (seq_t)k) + stirling[n-1][k-1];
        }
    }
    stirling_initialized = 1;
}

seq_t a(idx_t n) {
    init_stirling();

    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) d++;

    /* Compute offset along the diagonal and coordinates */
    idx_t offset = n - d * (d + 1) / 2;
    idx_t i = offset;
    idx_t j = d - offset;

    /* Compute A(i,j) */
    seq_t sum = 0;
    for (idx_t t = 0; t <= i; ++t) {
        /* indices for Stirling array */
        idx_t n1 = t + j;
        idx_t n2 = (i - t) + j;
        seq_t val1 = stirling[n1][j];
        seq_t val2 = stirling[n2][j];
        sum += val1 * val2;
    }
    return sum;
}
