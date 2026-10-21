/*
 * A258387  a(n) = (n+1)^n + n^(n-1).
 * https://oeis.org/A258387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^n + n^(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The offset of the sequence is 1, so values for n < 1 are not used. */
    if (n == 0) {
        return 0;  /* behaviour below the offset is irrelevant */
    }

    /* Ensure we operate in seq_t arithmetic from the start. */
    seq_t nn = (seq_t)n;

    /* Compute (n+1)^n and n^(n-1) using the provided big_pow helper. */
    seq_t term1 = big_pow(nn + (seq_t)1, nn);
    seq_t term2 = big_pow(nn, nn - (seq_t)1);

    /* Return the sum of the two terms. */
    return term1 + term2;
}
