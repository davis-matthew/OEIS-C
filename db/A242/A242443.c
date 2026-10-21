/*
 * A242443  Number of ways of writing n, a positive integer, as an unordered
 *          sum of a triangular number (A000217), an even square (A016742) and
 *          a generalized pentagonal number (A001318).
 * https://oeis.org/A242443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of representations of n as * a triangular number +
 *            an even square + a generalized pentagonal number * (unordered,
 *            components are distinct by type)
 * Range:     u8 n <= 3, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1). */
    seq_t count = 0;
    const seq_t N = (seq_t)n;

    for (idx_t k = 1;; ++k) {
        /* triangular number T_k = k*(k+1)/2 */
        seq_t tri = (seq_t)k * ((seq_t)k + 1) / 2;
        if (tri > N) break;

        for (idx_t m = 0;; ++m) {
            /* even square S_m = (2m)^2 = 4*m*m */
            seq_t sq = (seq_t)4 * (seq_t)m * (seq_t)m;
            seq_t sum = tri + sq;
            if (sum > N) break;

            seq_t p = N - sum;           /* remaining part for pentagonal */
            seq_t val = (seq_t)24 * p + 1;  /* 24*p + 1 should be a perfect square */
            seq_t root = floor_sqrt(val);
            if (root * root == val)       /* check generalized pentagonal */
                ++count;
        }
    }
    return count;
}
