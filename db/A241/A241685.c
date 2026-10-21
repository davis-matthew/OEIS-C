/*
 * A241685  The total number of squares and rectangles appearing in the
 *          Thue-Morse sequence logical matrices after n stages.
 * https://oeis.org/A241685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2^(n + 2) + 3 - (-1)^n)^2/72)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* 2^(n+2) */
    seq_t two_pow = pow_uint((uint64_t)2, n + 2);

    /* add 2, and +2 again if n is odd (because -(-1)^n is +1 when n is odd) */
    seq_t inc = (n & 1) ? (seq_t)2 : (seq_t)0;
    seq_t t = two_pow + 2 + inc;

    /* compute floor(t^2 / 72) without overflow for n where the result fits 64 bits */
    seq_t q = t / 72;
    seq_t r = t % 72;
    return q * t + (r * t) / 72;
}
