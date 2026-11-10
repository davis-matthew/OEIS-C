/*
 * A376452  Positions of numbers in A376450 that end in 0.
 * https://oeis.org/A376452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = position m (counting from m=0) of the n-th term of
 *            A376450 that ends in 0; found by search, using the fact that the
 *            last digit is 1 iff the greedy quarter-square (A002620)
 *            expansion of m uses 1
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A376452_ends_zero(idx_t m)
{
    if (m == 0) return 1;
    idx_t k = 2;
    while ((idx_t)(((k + 1) * (k + 1)) / 4) <= m) k++;
    idx_t rem = m;
    for (; k >= 3; --k) {
        idx_t q = (k * k) / 4;
        if (q <= rem) rem -= q;
    }
    /* quarter-squares for k>=3 are 2,4,6,...; then 2 (k=3) is the smallest.
       remaining rem is 0 or 1; 1 is used iff rem==1 */
    return rem != 1;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;
    for (;; ++m) {
        if (A376452_ends_zero(m)) {
            ++count;
            if (count == n) return (seq_t)m;
        }
    }
}
