/*
 * A366476  Number of possible chmess games at the end of the n-th ply.
 * https://oeis.org/A366476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, bref
 * Formula:   a(n) = 25^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* The number of possible chmess games after n plies is 25^n. */
    return pow_uint(25, n);
}
