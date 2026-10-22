/*
 * A388045  a(n) = Sum_{k=0..n} 2^k * binomial(n,k) * binomial(3*n+1,k).
 * https://oeis.org/A388045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(n,k) * binomial(3*n+1,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t binom_nk = 1;          /* binomial(n,0) */
    seq_t binom_3nk1k = 1;       /* binomial(3*n+1,0) */
    seq_t pow2 = 1;              /* 2^0 */

    /* precompute 3*n+1 once in seq_t form */
    seq_t tmp3n1 = (seq_t)3 * (seq_t)n + (seq_t)1;

    for (idx_t k = 0; k <= n; ++k) {
        /* term = 2^k * binom(n,k) * binom(3*n+1,k) */
        seq_t term = binom_nk * binom_3nk1k;
        term = term * pow2;
        result = result + term;

        if (k == n) break;      /* last term processed */

        /* update binom(n,k+1) = binom(n,k)*(n-k)/(k+1) */
        binom_nk = binom_nk * ((seq_t)n - (seq_t)k) / ((seq_t)(k + 1));

        /* update binom(3*n+1,k+1) = binom(3*n+1,k)*(3*n+1-k)/(k+1) */
        seq_t mult1 = tmp3n1 - (seq_t)k;          /* (3*n+1 - k)   */
        binom_3nk1k = binom_3nk1k * mult1 / ((seq_t)(k + 1));

        /* update power of two: pow2 = 2^(k+1) */
        pow2 = pow2 << 1;
    }

    return result;
}
