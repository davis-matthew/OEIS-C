/*
 * A245490  a(n) = n^(n^3).
 * https://oeis.org/A245490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(n^3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special case: define 0^0 = 1 as in the OEIS entry */
    if (n == 0) return (seq_t)1;

    seq_t nseq = (seq_t)n;        /* convert index to seq_t */
    seq_t exp  = nseq * nseq * nseq;   /* compute n^3 in seq_t */
    return big_pow(nseq, exp);   /* compute n^(n^3) */
}
