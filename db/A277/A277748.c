/*
 * A277748  Number of n X n QR matrices.
 * https://oeis.org/A277748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(n*(n-1)/2) * (2^n - n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* outside the offset; safe return */

    idx_t e = n * (n - 1) / 2;                /* compute n(n‑1)/2 as idx_t */
    seq_t pow_half = pow2(e);                 /* 2^(n(n-1)/2)              */
    seq_t pow_full = pow2(n);                 /* 2^n                      */
    seq_t term = pow_full - (seq_t)n;         /* 2^n - n                  */
    return pow_half * term;                  /* product gives a(n)        */
}
