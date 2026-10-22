/*
 * A386861  The total number of big inversions in all parking functions of
 *          length n.
 * https://oeis.org/A386861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(n+1)^(n-2)/4 (OEIS A386861)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2)          /* a(1) = 0; for n=0 or negative offset ignore */
        return 0;

    seq_t N = (seq_t)n;                 /* cast index to seq_t */
    seq_t pow = big_pow(N + 1, N - 2);  /* (n+1)^(n-2), exponent is non‑negative */

    seq_t term = N * (N - 1);           /* n*(n-1)   */
    term *= (N - 2);                    /* * (n-2)    */
    term *= pow;                        /* * (n+1)^(n-2) */
    term /= 4;                          /* divide by 4 (exact) */

    return term;
}
