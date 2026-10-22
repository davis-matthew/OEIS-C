/*
 * A319927  Numbers k such that the sum of the squares of the odd non-unitary
 *          divisors of k divides the sum of the squares of the non-unitary
 *          divisors of k.
 * https://oeis.org/A319927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that (sum of squares of odd
 *            non‑unitary divisors of k) divides (sum of squares of all
 *            non‑unitary divisors of k)
 * Range:     u8 n <= 35, u16 n <= 9313, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------------------------------------------------------- */
/* Utility functions (no external libraries needed)           */
/* ----------------------------------------------------------- */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"

/* Euclidean algorithm for unsigned 64‑bit integers. */

/* Integer square root: floor(sqrt(n)) for 0 <= n <= 2^64-1. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = 1ULL << 32;
    while (lo < hi)
    {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Determine whether k satisfies the A319927 condition.
   Returns true if the condition holds; odd_sum128 and all_sum128
   are only set for debugging and are not used afterwards. */
static bool is_good_k(uint64_t k, unsigned __int128 *odd_sum128,
                      unsigned __int128 *all_sum128)
{
    /* Non‑unitary divisors: pairs (d, k/d) with d != k/d and
       gcd(d, k/d) > 1.  Skip unitary divisors and the unitary
       divisor 1. */
    uint64_t limit = isqrt_u64(k);

    unsigned __int128 sum_odd = 0;
    unsigned __int128 sum_all = 0;

    for (uint64_t d = 2; d <= limit; ++d)
    {
        if (k % d != 0)
            continue;

        uint64_t d2 = k / d;
        uint64_t g = gcd_u64(d, d2);
        if (g == 1)                 /* unitary, skip */
            continue;

        /* d is non‑unitary */
        unsigned __int128 d_sq = (unsigned __int128)d * d;
        sum_all += d_sq;
        if (d & 1) sum_odd += d_sq;

        if (d2 != d)
        {
            unsigned __int128 d2_sq = (unsigned __int128)d2 * d2;
            sum_all += d2_sq;
            if (d2 & 1) sum_odd += d2_sq;
        }
    }

    *odd_sum128 = sum_odd;
    *all_sum128 = sum_all;

    /* Avoid division by zero: sum_odd must be > 0. */
    if (sum_odd == 0)
        return false;
    return (sum_all % sum_odd) == 0;
}

/* ----------------------------------------------------------- */
/* Main sequence function                                      */
/* ----------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Static cache: cached terms of the sequence in order. */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 1;

    if (n == 0)   /* undefined, but return 0 to keep type safety */
        return 0;

    /* Generate terms until we have at least n terms cached. */
    while (cache_sz < (size_t)n)
    {
        ++last_checked;

        unsigned __int128 odd_sum, all_sum;
        if (is_good_k(last_checked, &odd_sum, &all_sum))
        {
            /* Ensure capacity */
            if (cache_sz + 1 > cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                {
                    free(cache);
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the term (truncated to seq_t). */
            cache[cache_sz++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
