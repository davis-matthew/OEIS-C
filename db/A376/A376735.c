/*
 * A376735  a(n) = Sum_{k=0..floor(n/4)} (n-3*k+1) * binomial(n-3*k,k)^2.
 * https://oeis.org/A376735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/4⌋} (n-3k+1) * binomial(n-3k, k)^2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;            /* floor(n/4) */
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 3 * k;       /* n - 3k */
        seq_t b = binomial(m, k);  /* binomial(m,k) */
        seq_t term = (seq_t)(m + 1) * b * b;  /* (m+1) * b^2 */
        sum += term;
    }
    return sum;
}
