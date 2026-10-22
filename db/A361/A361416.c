/*
 * A361416  a(n) is the least integer z for which there is a triple (x,y,z)
 *          satisfying x^2 + n*x*y + y^2 = z^2 and 0 < x < y < z.
 * https://oeis.org/A361416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer z for which there exist positive integers x
 *            < y < z satisfying x^2 + n·x·y + y^2 = z^2
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 128‑bit unsigned values. */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;  /* avoid underflow */
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* defined only for n ≥ 1, but guard anyway */

    /* Pre‑compute (n²–4) as a signed __int128 to allow negative values when
       n = 1. */
    __int128 n128 = (__int128)n;
    __int128 coeff = n128 * n128 - 4; /* may be negative for n = 1 */

    for (idx_t z = 3; ; ++z) {                    /* z > 2   (since y ≥ 2) */
        __int128 z128 = (__int128)z;
        for (idx_t y = 1; y < z; ++y) {           /* y starts at 1; x will be checked */
            __int128 y128 = (__int128)y;

            /* Discriminant: D = (n²−4)·y² + 4·z²  (always ≥ 0 when a solution
               exists).  Treat it as unsigned for square‑root. */
            __int128 D128 = 4 * z128 * z128 + coeff * y128 * y128;
            if (D128 < 0) continue;              /* negative discriminant → no real root */

            unsigned __int128 D = (unsigned __int128)D128;
            unsigned __int128 sqrtD = isqrt128(D);
            if (sqrtD * sqrtD != D) continue;    /* D not a perfect square */

            __int128 delta = (__int128)sqrtD - n128 * y128;
            if (delta <= 0) continue;            /* would give x ≤ 0 */
            if ((delta & 1) != 0) continue;      /* delta must be even to give integer x */

            __int128 x = delta / 2;
            if (x > 0 && x < y) {                /* 0 < x < y < z   satisfied */
                return (seq_t)z;
            }
        }
    }
}
