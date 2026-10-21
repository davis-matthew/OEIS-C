/*
 * A262944  Number of ordered pairs (x,y) with x >= 0 and y > 0 such that n -
 *          x^4 - y*(y+1)/2 is a square or a pentagonal number.
 * https://oeis.org/A262944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of ordered pairs (x >= 0, y > 0) such that n - x^4 -
 *            y*(y+1)/2 is a square or a pentagonal number
 * Range:     u8 n <= 5, u16 n <= 1557, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer square root (floor) for arbitrary seq_t values. */
static inline seq_t isqrt(seq_t v)
{
    seq_t low = 0, high = v;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t sq = mid * mid;
        if (sq <= v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Test whether v is a perfect square. */
static inline bool is_square(seq_t v)
{
    return v == (isqrt(v) * isqrt(v));
}

/* Test whether v is a pentagonal number: v = k*(3*k-1)/2. */
static inline bool is_pentagonal(seq_t v)
{
    if (v < 0) return false;
    seq_t D = 1 + (seq_t)24 * v;
    seq_t sqrtD = isqrt(D);
    return (sqrtD * sqrtD == D) && ((1 + sqrtD) % 6 == 0);
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Loop over x with x^4 <= n. */
    for (idx_t x = 0; ; ++x) {
        seq_t x4 = (seq_t)x;
        x4 = x4 * x4 * x4 * x4;          /* x^4 */
        if (x4 > n) break;
        seq_t rem_after_x = n - x4;

        /* Loop over y with y*(y+1)/2 <= rem_after_x. */
        for (idx_t y = 1; ; ++y) {
            seq_t tri = (seq_t)y * (y + 1) / 2;  /* triangular number */
            if (tri > rem_after_x) break;
            seq_t s = rem_after_x - tri;
            if (is_square(s) || is_pentagonal(s))
                ++count;
        }
    }

    return count;
}
