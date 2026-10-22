/*
 * A362952  Sum of divisors of 5*n-1 of form 5*k+2.
 * https://oeis.org/A362952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑{ d | (5*n–1) , d ≡ 2 (mod 5) } d
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit value, using binary search and
   128‑bit intermediate arithmetic to avoid overflow. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n, mid, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Compute m = 5*n - 1, careful about overflow of idx_t. */
    uint64_t m = (uint64_t)5 * (uint64_t)n - 1ULL;

    uint64_t limit = isqrt_u64(m);
    seq_t sum = 0;

    for (uint64_t d = 1; d <= limit; ++d) {
        if (m % d != 0) continue;

        if (d % 5 == 2) sum += (seq_t)d;

        uint64_t e = m / d;
        if (e != d && e % 5 == 2) sum += (seq_t)e;
    }

    return sum;
}
