/*
 * A369398  Expansion of (1/x) * Series_Reversion( x / (1+x)^2 * (1-x^3) ).
 * https://oeis.org/A369398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/3⌋} binomial(n+k,k) *
 *            binomial(2*n+2, n-3*k)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 11, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t max_k = n / 3;
    idx_t m     = 2 * n + 2;
    seq_t sum   = 0;
    seq_t b1    = 1;                    /* binomial(n+0,0) */
    seq_t b2    = binomial(m, n);        /* binomial(2n+2, n) */

    for (idx_t k = 0; k <= max_k; ++k) {
        sum += b1 * b2;

        if (k == max_k) break;

        /* update b1 to binomial(n+k+1, k+1) */
        b1 = (b1 * (seq_t)(n + k + 1)) / (seq_t)(k + 1);

        /* update b2 to binomial(2n+2, n-3(k+1)) */
        idx_t r   = n - 3 * k;                       /* current r = n-3k */
        idx_t num = r * (r - 1) * (r - 2);           /* r*(r-1)*(r-2)   */
        idx_t den = (m - r + 1) * (m - r + 2) * (m - r + 3);
        b2 = (b2 * (seq_t)num) / (seq_t)den;
    }

    return sum / ((seq_t)n + 1);
}
