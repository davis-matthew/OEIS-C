/*
 * A262714  a(n) = a(n-1)*a(n-2) + 1, with a(0) = a(1) = 2.
 * https://oeis.org/A262714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*a(n-2) + 1, with a(0) = a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed_max = 0;
static bool init_done = false;

/* Ensure the cache can hold indices up to n */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    if (new_cap < cache_cap * 2)
        new_cap = cache_cap * 2;
    if (new_cap < 2)
        new_cap = 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();        /* out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) using memoised recurrence */
seq_t a(idx_t n)
{
    if (!init_done) {
        /* initialise base cases a(0) = a(1) = 2 */
        ensure_cache_size(1);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)2;
        computed_max = 1;
        init_done = true;
    }

    /* Extend cache and compute new terms if necessary */
    if (n > computed_max) {
        ensure_cache_size(n);
        for (idx_t k = computed_max + 1; k <= n; ++k) {
            cache[k] = cache[k - 1] * cache[k - 2] + (seq_t)1;
        }
        computed_max = n;
    }

    return cache[n];
}
