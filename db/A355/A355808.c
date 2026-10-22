/*
 * A355808  a(n) is the number at the apex of a triangle whose base contains
 *          the distinct powers of 2 summing to n (in ascending order), and
 *          each number in a higher row is the difference of the two numbers
 *          directly below it; a(0) = 0.
 * https://oeis.org/A355808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = apex of a triangle whose base is the distinct powers of 2
 *            summing to n. This implementation builds the base in ascending
 *            order and repeatedly replaces each row by the differences of
 *            adjacent elements until a single number remains
 * Range:     i8 n <= 127, i16 n <= 8192, i32 n <= 8192, i64 n <= 8192,
 *            i128 n <= 8192, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* collect exponents of set bits in ascending order */
    int bits[64];
    int k = 0;
    idx_t m = n;
    for (int pos = 0; m; ++pos, m >>= 1) {
        if (m & 1)
            bits[k++] = pos;
    }

    /* cur holds current row; size starts at k */
    seq_t cur[64];
    for (int i = 0; i < k; ++i)
        cur[i] = pow2((idx_t)bits[i]);

    int len = k;
    while (len > 1) {
        for (int i = 0; i < len - 1; ++i) {
            cur[i] = cur[i + 1] - cur[i];
        }
        --len;
    }
    return cur[0];
}
