/*
 * A362502  Least k > 0 such that (floor(sqrt(n*k)) + 1)^2 mod n is a square.
 * https://oeis.org/A362502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that (floor(sqrt(n*k)) + 1)^2 mod n is a
 *            square
 * Range:     u8 n <= 292, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* integer square root of a 128‑bit unsigned integer, floor(sqrt(x)) */
static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFFFFFFFFFULL, mid;
    while (lo <= hi)
    {
        mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == x) return mid;
        if (sq < x)
        {
            if (mid == 0xFFFFFFFFFFFFFFFFULL) break;
            lo = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* does x (0 <= x < 2^64) equal a perfect square? */
static inline bool is_square_uint64(uint64_t x)
{
    uint64_t r = isqrt_u128((unsigned __int128)x);
    return (unsigned __int128)r * r == x;
}

/* compute a(n) for n >= 1 */
static seq_t compute_a_internal(idx_t n)
{
    for (idx_t k = 1;; ++k)
    {
        unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)k;
        uint64_t m = isqrt_u128(prod);                /* floor(sqrt(n*k)) */
        uint64_t t_mod = (m + 1ULL) % n;               /* (m+1) mod n   */
        uint64_t val = (unsigned __int128)t_mod * t_mod % n; /* (m+1)^2 mod n */

        if (is_square_uint64(val))
            return (seq_t)k;
    }
}

/* Public API – indexed from 1 as in OEIS. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0 – return 0 */
    return compute_a_internal(n);
}
