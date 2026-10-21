/*
 * A295849  Number of nonnegative solutions to gcd(x,y,z) = 1 and x^2 + y^2 +
 *          z^2 <= n.
 * https://oeis.org/A295849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions (x,y,z) with
 *            gcd(x,y,z)=1 and x^2 + y^2 + z^2 <= n (OEIS A295849)
 *            Implementation: brute‑force enumeration of all triples with sum
 *            <= n, counting those whose coordinates are coprime
 * Range:     u8 n <= 61, u16 n <= 2770, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function runs in about O(n³/₂) time; it is fast enough for
   n up to ≈ 10 000 (≈ 10⁶ iterations).  */

#include <stdlib.h>
#include "../utils/gcd.h"

static idx_t _isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t x_limit = _isqrt_u64(n);
    for (idx_t x = 0; x <= x_limit; ++x) {
        idx_t xsq = x * x;
        idx_t rem1 = n - xsq;
        idx_t y_limit = _isqrt_u64(rem1);
        for (idx_t y = 0; y <= y_limit; ++y) {
            idx_t ysq = y * y;
            idx_t rem2 = rem1 - ysq;
            idx_t z_limit = _isqrt_u64(rem2);
            for (idx_t z = 0; z <= z_limit; ++z) {
                seq_t gxy = gcd((seq_t)x, (seq_t)y);
                seq_t g = gcd(gxy, (seq_t)z);
                if (g == (seq_t)1)
                    count++;
            }
        }
    }
    return count;
}
