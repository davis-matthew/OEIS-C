/*
 * A246863  Expansion of phi(x) * f(x^1, x^7) in powers of x where phi(), f()
 *          are Ramanujan theta functions.
 * https://oeis.org/A246863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the Euler transform of the periodic sequence c_k = [3,
 *            -4, 2, -1, 2, -3, 3, -2, 3, -3, 2, -1, 2, -4, 3, -2] of period
 *            16
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Periodic coefficients c_k (k = 1,2,…) */
static const int8_t c_coeff[16] =
{
    3, -4, 2, -1, 2, -3, 3, -2,
    3, -3, 2, -1, 2, -4, 3, -2
};

/* Return the n-th term of A246863 */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* Buffers for the intermediate coefficients */
    size_t len = (size_t)(n + 1);
    long long *b = (long long *)malloc(len * sizeof(long long));
    if (!b) return (seq_t)0;          /* failure: return 0 */

    /* Compute b_k = Σ_{d|k} d * c_d  (k = 1..n) */
    for (size_t k = 0; k < len; ++k) b[k] = 0;
    for (idx_t d = 1; d <= n; ++d) {
        int8_t cd = c_coeff[(d - 1) % 16];
        for (idx_t m = d; m <= n; m += d) {
            b[m] += (long long)d * (long long)cd;
        }
    }

    /* Compute the Euler transform entries e_k (k = 0..n) */
    seq_t *e = (seq_t *)malloc(len * sizeof(seq_t));
    if (!e) { free(b); return (seq_t)0; }
    e[0] = (seq_t)1;
    for (size_t i = 1; i <= n; ++i) {
        long long sum = 0;
        for (size_t k = 1; k <= i; ++k) {
            sum += b[k] * (long long)e[i - k];
        }
        /* The division is exact according to the definition. */
        e[i] = (seq_t)(sum / (long long)i);
    }

    seq_t result = e[(size_t)n];
    free(e);
    free(b);
    return result;
}
