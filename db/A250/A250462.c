/*
 * A250462  Number of (n+1)X(2+1) 0..1 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A250462
   Empirical recurrence (valid for n ≥ 7):
   a(n) = 4*a(n-1) + 3*a(n-2) - 10*a(n-3) + 4*a(n-5)
   Initial values:
       a(1)=31, a(2)=134, a(3)=557, a(4)=2322, a(5)=9651, a(6)=40124          */
#include <stdlib.h>

/* Cached values for previously computed indices.
   The cache grows as needed and is reused for arbitrary-order calls.   */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* largest index stored in cache (0 means none)   */
static size_t cache_alloc = 0; /* allocated size of the cache array             */

/* Initial values for indices 1..6; index 0 is a dummy to keep 1‑based access. */
static const seq_t init[7] = {
    0ULL,      /* dummy for index 0 */
    31ULL,     /* a(1) */
    134ULL,    /* a(2) */
    557ULL,    /* a(3) */
    2322ULL,   /* a(4) */
    9651ULL,   /* a(5) */
    40124ULL   /* a(6) */
};

seq_t a(idx_t n)
{
    /* Handle the first six values directly. */
    if (n <= 6)
        return init[(size_t)n];

    /* Initialize the cache on the first call. */
    if (cache_len == 0) {
        cache_alloc = 7;                         /* indices 0..6 */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_alloc);
        if (!cache) abort();
        for (size_t i = 1; i <= 6; ++i)
            cache[i] = init[i];
        cache_len = 6;
    }

    /* Grow the cache if the requested index is beyond its current capacity. */
    if ((size_t)n + 1 > cache_alloc) {
        size_t new_alloc = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_alloc);
        if (!tmp) abort();
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* Compute any missing entries up to n using the recurrence. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        /* a(i) = 4*a(i-1) + 3*a(i-2) - 10*a(i-3) + 4*a(i-5) */
        seq_t val = 4 * cache[i-1] + 3 * cache[i-2]
                    - 10 * cache[i-3] + 4 * cache[i-5];
        cache[i] = val;
    }

    cache_len = (size_t)n;
    return cache[(size_t)n];
}
