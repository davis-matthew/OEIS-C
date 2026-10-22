/*
 * A382486  Product of distinct prime divisors of n that are <= sqrt(n).
 * https://oeis.org/A382486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = product of distinct prime divisors of n that are <=
 *            sqrt(n)
 * Range:     u8 n <= 272, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Integer square‑root, floor of sqrt(x).  */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid <= x / mid) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }

    seq_t prod = (seq_t)1;
    uint64_t nn = (uint64_t)n;
    uint64_t sqrt_n = isqrt_u64(nn);

    /* handle factor 2 */
    if ((nn & 1ULL) == 0) {
        if (2 <= sqrt_n) {
            prod *= (seq_t)2;
        }
        while ((nn & 1ULL) == 0) {
            nn >>= 1;
        }
    }

    /* odd factors */
    for (uint64_t d = 3; d <= sqrt_n; d += 2) {
        if (nn % d == 0) {
            if (d <= sqrt_n) {
                prod *= (seq_t)d;
            }
            while (nn % d == 0) {
                nn /= d;
            }
        }
    }

    return prod;
}
