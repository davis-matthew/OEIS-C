/*
 * A389933  a(n) = Sum_{k=0..n} binomial(n+k+1,n-k) * Fibonacci(k+1).
 * https://oeis.org/A389933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k+1,n-k) * Fibonacci(k+1) *
 *            Recurrence: a(n) = 5*a(n-1) - 7*a(n-2) + 5*a(n-3) - a(n-4) *
 *            Initial values: a(0)=1, a(1)=3, a(2)=9, a(3)=29
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 966 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *cache = NULL;          /* Cached sequence values */
static size_t  cache_cap = 0;         /* Allocated capacity of cache */
static size_t  cache_size = 0;        /* Number of values already computed */

/* Ensure that cache contains values up to index n (inclusive), computing
   any missing terms using the recurrence.  The function is safe for
   repeated or out‑of‑order calls. */
static void
ensure_cache(idx_t n)
{
    /* Grow the underlying array if necessary */
    if ((size_t)(n + 1) > cache_cap)
        grow_seq(&cache, &cache_cap, (size_t)(n + 1));

    /* Fill in the base cases if this is the first call */
    if (cache_size == 0) {
        cache[0] = 1;
        cache_size = 1;
    }

    /* Compute missing values up to n */
    if (cache_size <= 1) {          /* a(1) */
        cache[1] = 3;
        cache_size = 2;
    }
    if (cache_size <= 2) {          /* a(2) */
        cache[2] = 9;
        cache_size = 3;
    }
    if (cache_size <= 3) {          /* a(3) */
        cache[3] = 29;
        cache_size = 4;
    }

    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        /* a(i) = 5*a(i-1) - 7*a(i-2) + 5*a(i-3) - a(i-4) */
        seq_t t = mul_small(cache[i - 1], 5);
        t -= mul_small(cache[i - 2], 7);
        t += mul_small(cache[i - 3], 5);
        t -= cache[i - 4];
        cache[i] = t;
    }
    cache_size = (size_t)(n + 1);
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
