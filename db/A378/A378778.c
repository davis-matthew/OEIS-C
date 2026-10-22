/*
 * A378778  a(n) = n^2 * 2^n * binomial(3*n, n).
 * https://oeis.org/A378778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * 2^n * binomial(3*n, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Compute C(3*n, n) as the product:
         Π_{i=1..n} (2*n + i) / i
       This keeps intermediate values small and exact. */
    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        binom = binom * ((seq_t)(2 * n + i));
        binom = binom / (seq_t)i;
    }

    /* Compute the remaining factors n^2 * 2^n. */
    seq_t res = (seq_t)n * (seq_t)n;   /* n^2 */
    res *= pow2(n);                    /* times 2^n */
    res *= binom;                      /* times C(3n, n) */

    return res;
}
