/*
 * A248217  a(n) = 8^n - 2^n.
 * https://oeis.org/A248217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 2^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n and 2^(3n), then subtract. */
    seq_t two_n     = pow2(n);
    seq_t two_3n    = pow2((idx_t)(n * 3ULL));
    return two_3n - two_n;
}
