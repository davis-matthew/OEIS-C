/*
 * A368784  a(0) = 1. For n > 0, a(n) is the smallest integer k > n such that
 *          (Sum_{i = 1..n} i)/(Sum_{i = n + 1..k} i) < 1/n.
 * https://oeis.org/A368784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(-1/2 + sqrt(4*n^3 + 8*n^2 + 4*n + 1)/2) + 1
 * Range:     u8 n <= 39, u16 n <= 1624, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only integer arithmetic. */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for x up to (2^64)^2. */
static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t low  = 0;
    uint64_t high = (uint64_t)1 << 32;   /* high^2 fits in 128 bits */

    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 == x) return mid;
        else if (mid2 < x) low = mid + 1;
        else high = mid - 1;
    }
    return high;   /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Compute D = 4*n^3 + 8*n^2 + 4*n + 1 using 128‑bit arithmetic. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 D  = 4*nn*nn*nn + 8*nn*nn + 4*nn + 1;

    /* s = floor(sqrt(D)) */
    uint64_t s = isqrt_u128(D);

    /* a(n) = (s + 1) / 2; integer division gives the required floor. */
    unsigned __int128 ans = (s + 1) / 2;

    return (seq_t)ans;
}
