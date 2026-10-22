/*
 * A395787  Expansion of (Sum_{k>=0} binomial(4*k+2,k) * x^k)^4.
 * https://oeis.org/A395787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(k+2,2) * binomial(4*n+11,n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

/* Computes the n-th term of OEIS sequence A395787.
   idx_t is an unsigned 64‑bit integer type.
   seq_t is an unsigned integer type of arbitrary width or an
   arbitrary‑precision integer class. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* a(0) = 1 */

    /* N = 4*n + 11, used in the binomial in the summation. */
    idx_t N = 4 * n + 11;

    seq_t sum   = (seq_t)0;
    seq_t pow3  = (seq_t)1;            /* 3^k, starts at k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(4*n+11, n-k) */
        idx_t m       = n - k;                 /* current m = n-k */
        seq_t cur_bin = binomial(N, m);

        /* binomial(k+2,2) = (k+2)*(k+1)/2 */
        seq_t binom_k2 = (seq_t)(k + 2) * (seq_t)(k + 1) / (seq_t)2;

        /* term = 3^k * binomial(k+2,2) * binomial(4*n+11,n-k) */
        sum += pow3 * binom_k2 * cur_bin;

        /* update 3^k for next iteration */
        pow3 *= (seq_t)3;
    }

    return sum;
}
