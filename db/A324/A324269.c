/*
 * A324269  a(n) = 3*11^(2*n).
 * https://oeis.org/A324269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*121^n
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   mul_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Compute 121^n in arbitrary‑precision */
    seq_t power = seq_pow((seq_t)121, n);
    /* Multiply the power by 3 */
    return mul_small(power, 3);
}
