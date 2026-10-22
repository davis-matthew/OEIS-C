/*
 * A307813  a(n) = (5/32)*4^n - floor((n^2 + 1)/2)*2^(n - 2).
 * https://oeis.org/A307813
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5/32)*4^n - floor((n^2 + 1)/2)*2^(n - 2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 3 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n < 3 the offset is before the list of terms; return 0 to avoid
       negative exponents in pow2.  The behaviour below the offset is
       irrelevant for the tests. */
    if (n < 3) return 0;

    /* part1 = 5 * 2^(2n-5)  because 4^n = 2^(2n) and 5/32 = 5 * 2^-5 */
    seq_t part1 = 5 * pow2((idx_t)2 * n - 5);

    /* f = floor((n^2 + 1)/2) */
    seq_t fn_sq = (seq_t)n * (seq_t)n;
    seq_t f = (fn_sq + 1) / 2;

    /* part2 = f * 2^(n-2) */
    seq_t part2 = f * pow2(n - 2);

    return part1 - part2;
}
