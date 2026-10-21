/*
 * A276659  Accumulation of the upper left triangle used in binomial transform
 *          of nonnegative integers.
 * https://oeis.org/A276659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2^(n+2) - n - 3)/2
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow_val = pow2(n + 2);              /* 2^(n+2) */
    seq_t term     = pow_val - (seq_t)n - (seq_t)3;
    return ((seq_t)n * term) / 2;
}
