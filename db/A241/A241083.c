/*
 * A241083  LCM of n and largest integer <= sqrt(n).
 * https://oeis.org/A241083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = lcm(n, floor(sqrt(n))) = n / gcd(n, floor(sqrt(n))) *
 *            floor(sqrt(n))
 * Range:     u8 n <= 42, u16 n <= 1638, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Simple integer square‑root by binary search */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t low = 0, high = n, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid > 0 && mid > n / mid) { /* mid*mid > n */
            high = mid - 1;
        } else {
            res = mid;      /* mid*mid <= n */
            low = mid + 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset 1 – unused case */
    idx_t s = int_sqrt(n);
    seq_t sn = (seq_t)n;
    seq_t ss = (seq_t)s;
    seq_t g  = gcd(sn, ss);
    return (sn / g) * ss;
}
