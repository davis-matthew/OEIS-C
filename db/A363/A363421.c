/*
 * A363421  a(n) = Sum_{k=0..n}(n^[not(k | n)] - n^[k | n]), where '[ ]'
 *          denotes the Iverson bracket.
 * https://oeis.org/A363421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n^2 - 2*(n - 1)*tau(n) - 1 (tau = divisor_count_u64)
 * Range:     i8 n <= 16, i16 n <= 186, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9981 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Use a larger intermediate type to avoid overflow */
    __int128 iv = (__int128)n;
    __int128 tau = (__int128)num_divisors((uint64_t)n);

    __int128 result = iv * iv
                      - (__int128)2 * (iv - 1) * tau
                      - 1;

    return (seq_t)result;
}
