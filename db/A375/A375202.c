/*
 * A375202  a(n) is the least integer x >= 0 such that n = x^2 + y^2 + z^2 for
 *          some integers y, z, or -1 if there is no such x.
 * https://oeis.org/A375202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = the smallest non‑negative integer x such that n = x^2 +
 *            y^2 + z^2 for some integers y, z (return -1 if none)
 * Range:     i8 n <= 63, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_sum_two_squares_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_sum_two_squares_uint64.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* floor_sqrt works with seq_t; its result fits in idx_t */
    idx_t max_x = (idx_t)floor_sqrt(N);

    for (idx_t x = 0; x <= max_x; ++x) {
        seq_t m = N - (seq_t)x * (seq_t)x;
        /* m ≥ 0 by construction */
        if (is_sum_two_squares_uint64((uint64_t)m))
            return (seq_t)x;
    }
    return -1;
}
