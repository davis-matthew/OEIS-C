/*
 * A395186  a(n) is the number of pairwise incongruent triangles with distinct
 *          integer sides and positive integer area and longest side of length
 *          n.
 * https://oeis.org/A395186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairwise incongruent triangles with distinct
 *            integer sides, positive integer area, and longest side of length
 *            n
 * Range:     u8 n <= 2626, u16 n <= 2626, u32 n <= 2626, u64 n <= 2626,
 *            u128 n <= 2626, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 128‑bit numbers using binary search */
static inline uint64_t isqrt_u128(__uint128_t x)
{
    uint64_t lo = 0, hi = (1ULL << 32) - 1;          /* sqrt(2^64-1) < 2^32 */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* For n < 3 no triangle with a < b < n exists */
    if (n < 3)
        return (seq_t)0;

    /* Count triangles (a,b,n) with a<b<n, a+b>n, integer area */
    uint64_t count = 0;

    for (idx_t a = 1; a <= n - 2; ++a) {
        /* b must satisfy a < b < n and a + b > n => b > n - a */
        uint64_t b_min = a + 1;
        uint64_t tmp = n - a + 1;          /* n - a + 1 = (n - a) + 1 */
        if (tmp > b_min)
            b_min = tmp;
        if (b_min >= n)
            continue;

        for (idx_t b = b_min; b <= n - 1; ++b) {
            /* Semiperimeter must be integer: a + b + n even. */
            if (((a + b + n) & 1u) != 0u)
                continue;

            /* Compute terms for Heron's formula */
            idx_t su = (a + b + n) >> 1;          /* semiperimeter (integer) */
            idx_t t1 = su - a;
            idx_t t2 = su - b;
            idx_t t3 = su - n;

            /* Compute product in 128 bit to avoid overflow */
            __uint128_t prod = (__uint128_t)su * t1 * t2 * t3;

            /* Integer area iff product is a perfect square */
            uint64_t r = isqrt_u128(prod);
            if ((__uint128_t)r * r == prod)
                ++count;
        }
    }

    return (seq_t)count;
}
