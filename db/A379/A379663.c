/*
 * A379663  a(n) is the number of integer-sided triangles whose sides are in
 *          geometric progression with smallest side n.
 * https://oeis.org/A379663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer‑sided triangles whose sides are n, n*r,
 *            n*r² with n integer and r rational, 1 ≤ r ≤ (1+√5)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* floor_sqrt_u64: floor(sqrt(x)) for unsigned 64‑bit integer x */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = 1ULL << 32;          /* 2^32 > sqrt(2^64−1) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* floor_sqrt128: floor(sqrt(x)) for unsigned __int128 x */
static idx_t floor_sqrt128(unsigned __int128 x)
{
    idx_t lo = 0, hi = 1ULL << 34;          /* > sqrt(5*2^64) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, but safeguard */
    seq_t count = 0;
    idx_t dmax = floor_sqrt_u64(n);

    for (idx_t d = 1; d <= dmax; ++d) {
        idx_t d2 = d * d;
        if (n % d2 != 0) continue;   /* d² must divide n */

        /* compute floor( sqrt(5*d²) ) */
        unsigned __int128 val = (unsigned __int128)d * d * 5;
        idx_t sqrt5d = floor_sqrt128(val);

        /* upper bound for c: floor(d * φ) = (d + floor(d*√5)) / 2 */
        idx_t upper = (d + sqrt5d) / 2;

        for (idx_t c = d; c <= upper; ++c) {
            if (gcd((seq_t)c, (seq_t)d) == 1)
                ++count;
        }
    }
    return count;
}
