/*
 * A349835  Expansion of (1 + 4*x)/sqrt(1 - 4*x).
 * https://oeis.org/A349835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n,n) + 4*binomial(2*(n-1),n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t term1 = binomial(n << 1, n);                /* binomial(2*n, n) */
    seq_t term2 = binomial((n - 1) << 1, n - 1);      /* binomial(2*(n-1), n-1) */

    return term1 + (seq_t)4 * term2;
}
