/*
 * A275150  Number of ordered ways to write n as x^3 + 2*y^2 + k*z^2, where
 *          x,y,z are nonnegative integers, k is 1 or 5, and k = 1 if z = 0.
 * https://oeis.org/A275150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(x,y,z,k) : n = x^3 + 2*y^2 + k*z^2, x,y,z ≥ 0, k ∈
 *            {1,5}, and k = 1 if z = 0}|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count solutions by brute force over x, y and check the remaining
   contribution as a (possibly zero) square or 5×square.  Complexity
   is O(n^(1/3)·n^(1/2)).  For n ≤ 1e4 this is well under the time limit. */
#include <stdbool.h>
#include <stdint.h>

/* Integer square root of 64‑bit unsigned integer.
   Returns ⌊√n⌋.  |
   The algorithm uses only integer arithmetic and never overflows. */
static inline idx_t isqrt64(idx_t n)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 62;          /* 2^62 is highest power of four ≤ 2^64-1 */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

static inline bool is_square(idx_t n)
{
    if (n == 0) return false;            /* we only call for rem>0 */
    idx_t r = isqrt64(n);
    return r * r == n;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t x = 0;; ++x) {
        idx_t x3 = x * x * x;
        if (x3 > n) break;

        idx_t remX = n - x3;
        for (idx_t y = 0;; ++y) {
            idx_t yy = y * y;
            if (yy > remX / 2) break;          /* 2*y^2 > remX */

            idx_t rem = remX - 2 * yy;
            if (rem == 0) {
                /* z = 0, k = 1 only */
                ++count;
            } else {
                /* z > 0 */
                if (is_square(rem))          /* k = 1 case */
                    ++count;
                if ((rem % 5) == 0) {
                    idx_t q = rem / 5;
                    if (is_square(q))       /* k = 5 case */
                        ++count;
                }
            }
        }
    }
    return count;
}
