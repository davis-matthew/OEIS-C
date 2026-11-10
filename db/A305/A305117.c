/*
 * A305117  a(n) = A304651(n)/4.
 * https://oeis.org/A305117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A304651(n)/4, where A304651(n) = number of coprime pairs
 *            (x,y) with x^2+y^2 <= n; computed by direct enumeration.
 *            (Equivalently a(n) = a(n-1) + A000089(n).)
 * Range:     u8 n <= 532, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9323 (2026-11-08)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static inline idx_t A304651_isqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 1;
    idx_t high = n >> 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid > n / mid) {
            high = mid - 1;
        } else {
            if (mid + 1 > n / (mid + 1))
                return mid;
            low = mid + 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t count = 0;   /* A304651(n)/4 */
    idx_t r = A304651_isqrt(n);
    for (idx_t x = 0; x <= r; ++x) {
        idx_t rem = n - x * x;
        idx_t ymax = A304651_isqrt(rem);
        idx_t limit = (ymax < x) ? ymax : x;
        for (idx_t y = 0; y <= limit; ++y) {
            if (x == 0 && y == 0) continue;
            if (gcd((seq_t)x, (seq_t)y) != 1) continue;
            if (x == 0 || y == 0 || x == y)
                count += (seq_t)1;
            else
                count += (seq_t)2;
        }
    }
    return count;
}
