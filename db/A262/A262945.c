/*
 * A262945  Number of ordered pairs (x,y) with x >= 0 and y >= 0 such that n -
 *          x^4 - 2*y^2 is a triangular number or a pentagonal number.
 * https://oeis.org/A262945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y) | x≥0, y≥0, n - x^4 - 2*y^2 is a triangular or
 *            pentagonal number}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer using the binary method.
   Returned value is floor(sqrt(n)). */
static idx_t isqrt_u64(idx_t n)
{
    idx_t a = 0, b = 1ULL << 32;
    while (b) {
        if (a + b <= n / (a + b))
            a += b;
        b >>= 1;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t   count = 0;
    /* Loop over x with x^4 ≤ n; for n up to about 10 000, x is tiny. */
    for (idx_t x = 0; ; ++x) {
        idx_t x2 = x * x;
        idx_t x4 = x2 * x2;
        if (x4 > n) break;

        /* 2*y^2 ≤ n - x^4  ⇒  y^2 ≤ (n - x^4)/2 */
        idx_t max_y_sq = (n - x4) / 2;
        for (idx_t y = 0; y * y <= max_y_sq; ++y) {
            idx_t rem = n - x4 - 2 * (y * y);
            if (rem > n) continue;                     /* safety, should not happen */

            /* Triangular test: 8*rem + 1 is a perfect square. */
            idx_t tval = 8 * rem + 1;
            idx_t sqrt_t = isqrt_u64(tval);
            if (sqrt_t * sqrt_t == tval) {
                ++count;
                continue;
            }

            /* Pentagonal test: 24*rem + 1 is a perfect square and (sqrt + 1) is divisible by 6. */
            idx_t pval = 24 * rem + 1;
            idx_t sqrt_p = isqrt_u64(pval);
            if (sqrt_p * sqrt_p == pval && (sqrt_p + 1) % 6 == 0) {
                ++count;
            }
        }
    }
    return count;
}
