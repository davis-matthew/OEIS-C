/*
 * A397371  a(n) = n times the square root of the smallest square number
 *          greater than n.
 * https://oeis.org/A397371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1 + floor(sqrt(n)))
 * Range:     u8 n <= 36, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* floor_sqrt of n */
    idx_t k = _floor_isqrt_u64(n);
    /* compute a(n) = n * (1 + k) */
    seq_t n_seq = (seq_t)n;
    seq_t k_seq = (seq_t)k;
    return n_seq * (k_seq + (seq_t)1);
}
