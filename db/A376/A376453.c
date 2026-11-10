/*
 * A376453  Positions of numbers in A376450 that end in 1.
 * https://oeis.org/A376453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th m >= 1 such that A376450(m) ends in 1, i.e. the
 *            greedy quarter-square (A002620) expansion of m uses the place
 *            value 1; computed by search
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A376453_ends_in_1(idx_t m)
{
    idx_t k = 2;
    while ((k + 1) * (k + 1) / 4 <= m) k++;
    idx_t rem = m;
    int used1 = 0;
    for (; k >= 2; --k) {
        idx_t q = k * k / 4;
        if (q <= rem) {
            rem -= q;
            if (k == 2) used1 = 1;
        }
    }
    return used1;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;
    while (count < n) {
        ++m;
        if (A376453_ends_in_1(m)) ++count;
    }
    return (seq_t)m;
}
