/*
 * A260671  Expansion of theta_3(q) * theta_3(q^15) in powers of q.
 * https://oeis.org/A260671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x, y) of x^2 + 15*y^2 = n
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count integer solutions to x^2 + 15*y^2 = n for 0 <= n
 * The algorithm runs in O(sqrt(n)).  We use integer square root
 * computed via 128‑bit arithmetic to avoid overflow.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned integers.
 * Returns floor(sqrt(v)).  Uses 128‑bit multiplication to avoid overflow. */
static inline idx_t _isqrt_u64(idx_t v)
{
    idx_t r = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t bit = (idx_t)1 << i;
        idx_t t = r | bit;
        if ((unsigned __int128)t * t <= v)
            r = t;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t count = 0;          /* result */
    /* maximum |y| is floor(sqrt(n/15)) */
    idx_t max_y = _isqrt_u64(n / 15);

    /* y = 0 case */
    {
        idx_t rem = n;
        idx_t k = _isqrt_u64(rem);
        if ((unsigned __int128)k * k == rem) {
            if (k == 0)
                count += 1;   /* (0,0) */
            else
                count += 2;   /* (+k,0), (-k,0) */
        }
    }

    /* y > 0 cases */
    for (idx_t y = 1; y <= max_y; ++y) {
        idx_t yy = y * y;
        idx_t rem = n - 15 * yy;
        if (rem < 0) break;          /* safety, though not needed */
        idx_t k = _isqrt_u64(rem);
        if ((unsigned __int128)k * k == rem) {
            if (rem == 0)
                count += 2;           /* (0,±y) */
            else
                count += 4;           /* (+k,±y), (-k,±y) */
        }
    }

    return count;
}
