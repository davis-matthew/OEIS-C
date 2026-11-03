/*
 * A321885  a(1) = 1, a(n) = n + d(a(n-1)).
 * https://oeis.org/A321885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 83 terms through n = 936 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A321885:
 *
 *   a(1) = 1
 *   a(n) = n + d(a(n-1))   for n ≥ 2
 *   where d(x) = number of positive divisors of x
 *
 * The implementation memoises previously computed terms to answer
 * arbitrary call order in O(1) after initial O(n) fill.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *cache = NULL;        /* cache of a(k), index zero = a(1) */
static size_t cache_size = 0;      /* number of entries currently stored */
static size_t cache_capacity = 0;  /* capacity of allocated cache */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *p = mem_resize(seq_t, cache, new_cap);
    if (!p)
        exit(EXIT_FAILURE);
    cache = p;
    cache_capacity = new_cap;
}

/* Count positive divisors of x. */
static seq_t divisor_count(seq_t x)
{
    seq_t sq = floor_sqrt(x);
    seq_t d = 0;
    for (seq_t i = 1; i <= sq; ++i) {
        if (x % i == 0) {
            ++d;
            seq_t j = x / i;
            if (j != i)
                ++d;
        }
    }
    return d;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1, but guard anyway */
        return 0;

    if ((size_t)n > cache_size) {
        /* Grow cache to accommodate up to n */
        ensure_capacity((size_t)n);

        /* Initialise first term if cache empty */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;   /* a(1) = 1 */
            cache_size = 1;
        }

        /* Compute remaining terms up to n */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            seq_t prev = cache[k - 2];        /* a(k-1) */
            seq_t d = divisor_count(prev);   /* d(a(k-1)) */
            cache[k - 1] = (seq_t)k + d;      /* a(k) = k + d */
        }
        cache_size = (size_t)n;
    }
    return cache[n - 1];
}
