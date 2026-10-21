/*
 * A283776  Numbers k such that floor(k*sqrt(3)) is odd.
 * https://oeis.org/A283776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A283776: numbers k such that floor(k*sqrt(3)) is odd.
   Implementation by incremental search: a(n) is the n‑th integer k with
   floor(k*sqrt(3)) odd. */

#include <stdint.h>
#include <stddef.h>

static seq_t cache[20000];   /* cached terms */
static idx_t cache_len = 0;  /* number of terms stored */
static idx_t last_k = 0;     /* last k examined */

/* 64‑bit integer square root (floor). */
static inline uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo < hi)
    {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > n / mid)          /* avoid overflow of mid*mid */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* sequence is defined for n ≥ 1 */
        return (seq_t)0;

    while (cache_len < n)
    {
        ++last_k;

        /* Compute floor(k*sqrt(3)) = floor(sqrt(3*k^2)) */
        uint64_t t = 3ULL * (uint64_t)last_k * (uint64_t)last_k;
        uint64_t f = isqrt_uint64(t);

        if ((f & 1ULL) != 0)          /* odd? */
            cache[cache_len++] = (seq_t)last_k;
    }

    return cache[n - 1];
}
