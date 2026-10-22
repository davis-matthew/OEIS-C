/*
 * A369027  a(n) = floor(n^2 * (n - 1)^(n - 1) / 2).
 * https://oeis.org/A369027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2 * (n - 1)^(n - 1) / 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)0;
    }

    seq_t nm   = (seq_t)n;      /* n */
    seq_t nm1  = nm - 1;        /* n-1 */
    seq_t power = big_pow(nm1, nm1); /* (n-1)^(n-1) */

    seq_t n2    = nm * nm;      /* n^2 */
    seq_t prod  = n2 * power;   /* n^2 * (n-1)^(n-1) */
    return prod / 2;            /* floor division by 2 (exact) */
}
