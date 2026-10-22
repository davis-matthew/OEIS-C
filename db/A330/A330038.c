/*
 * A330038  a(1) = 1, a(n) = [n/2] + a([n/2]) + a([(n+1)/2]) for n > 1, where
 *          [x] = floor(x).
 * https://oeis.org/A330038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = floor(n/2) + a(floor(n/2)) + a(floor((n+1)/2))
 *            for n > 1 (OEIS A330038)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;          /* 1‑based index: cache[1] = a(1) */
static size_t cache_cap = 0;         /* current capacity of cache */
static size_t max_computed = 0;      /* largest index already computed */

/* Allocate or expand the cache so that indices up to n are valid. */
static void ensure_cache_size(idx_t n)
{
    size_t need = (size_t)n + 1;     /* indices 0..n, we ignore 0 */
    if (cache_cap < need) {
        ensure_capacity(need, &cache_cap, (void **)&cache, sizeof(seq_t));
        /* new entries beyond the old size are default‑uninitialized;
           initialise them to 0 for safety. */
        for (size_t i = max_computed + 1; i < cache_cap; ++i)
            cache[i] = (seq_t)0;
    }
}

/* Public sequence function.  Calls may be in any order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n < 1, return 0 safely */

    ensure_cache_size(n);

    /* Initialize the base case if it hasn't been done yet. */
    if (max_computed < 1) {
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    /* Compute any missing terms up to n. */
    if (max_computed < (size_t)n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            idx_t half = i / 2;                 /* floor(i/2)   */
            idx_t half_plus = (i + 1) / 2;      /* floor((i+1)/2) */
            cache[i] = (seq_t)half + cache[half] + cache[half_plus];
        }
        max_computed = (size_t)n;
    }

    return cache[n];
}
