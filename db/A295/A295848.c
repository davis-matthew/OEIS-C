/*
 * A295848  Number of nonnegative solutions to (x,y,z) = 1 and x^2 + y^2 + z^2
 *          = n.
 * https://oeis.org/A295848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = number of ordered non‑negative integer triples (x,y,z)
 *            with gcd(x,y,z)=1 and x^2 + y^2 + z^2 = n
 * Range:     u8 n <= 9433, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* integer square root using Newton's method, returns floor(sqrt(n)) */
static inline idx_t isqrt_idx(idx_t n)
{
    if (n <= 1)
        return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* iterate x */
    for (idx_t x = 0; ; ++x) {
        idx_t x2 = x * x;
        if (x2 > n)
            break;
        /* iterate y */
        for (idx_t y = 0; ; ++y) {
            idx_t y2 = y * y;
            if (x2 + y2 > n)
                break;
            idx_t z2 = n - x2 - y2;
            idx_t z = isqrt_idx(z2);
            if (z * z != z2)     /* not a perfect square */
                continue;
            /* gcd(x, y, z) == 1? */
            if (gcd_idx(gcd_idx(x, y), z) == 1)
                ++count;
        }
    }
    return count;
}
