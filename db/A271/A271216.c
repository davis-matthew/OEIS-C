/*
 * A271216  a(n) = 2^n * floor(n/2)!
 * https://oeis.org/A271216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * floor(n/2)!
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 27, u128 n <= 48,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    idx_t half = n >> 1;            /* floor(n/2) */
    seq_t p2   = pow2(n);           /* 2^n */
    seq_t f    = factorial(half);   /* floor(n/2)! */
    return p2 * f;
}
