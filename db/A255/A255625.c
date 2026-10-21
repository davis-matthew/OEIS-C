/*
 * A255625  Number of length n+3 0..2 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{8} p_k * C(n+7-k, 8) * where the polynomial
 *            coefficients are * p = {81, -486, 1458, -2577, 2766, -1790, 672,
 *            -135, 12} * C(m, 8) is the binomial coefficient (m choose 8). *
 *            This implements the exact (not empirical) definition of A255625
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 52, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* coefficients of the numerator polynomial */
    const int p[9] = {81, -486, 1458, -2577, 2766, -1790, 672, -135, 12};

    seq_t pos = 0;  /* sum of positive contributions */
    seq_t neg = 0;  /* sum of negative contributions (stored as positive) */

    for (int k = 0; k < 9; ++k) {
        idx_t m = n + 7 - k;        /* m = n+7-k  */
        if (m < 8)                  /* binomial is zero for m<8 */
            continue;

        /* compute binom(m, 8) iteratively */
        seq_t bin = 1;
        for (int i = 1; i <= 8; ++i) {
            seq_t mul = (seq_t)(m - 8 + i);  /* numerator factor */
            bin = (bin * mul) / (seq_t)i;    /* exact division */
        }

        if (p[k] >= 0) {
            pos += bin * (seq_t)p[k];
        } else {
            neg += bin * (seq_t)(-p[k]);
        }
    }

    return pos - neg;   /* final result is non‑negative */
}
