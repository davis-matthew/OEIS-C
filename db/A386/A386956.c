/*
 * A386956  a(n) = Sum_{k=0..n} (k+1) * 8^k * binomial(2*n+1,n-k).
 * https://oeis.org/A386956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * 8^k * binomial(2*n+1, n-k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* 2*n + 1 in idx_t.  Use a shift to avoid overflow with uint64_t. */
    idx_t two_n_plus_1 = (idx_t)(n << 1);
    two_n_plus_1 += (idx_t)1;

    seq_t sum   = (seq_t)0;
    seq_t pow8  = (seq_t)1;          /* 8^k */
    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        idx_t m = n - k;                 /* choose n-k */
        seq_t bin = binomial(two_n_plus_1, m);
        seq_t term = pow8 * bin;
        term *= (seq_t)(k + (idx_t)1);
        sum += term;
        pow8 *= (seq_t)8;                /* next power of 8 */
    }
    return sum;
}
