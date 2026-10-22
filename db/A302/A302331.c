/*
 * A302331  a(0)=1, a(1)=141; for n>1, a(n) = 142*a(n-1) - a(n-2).
 * https://oeis.org/A302331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 141, a(n) = 142*a(n-1) - a(n-2) for n > 1 */
#include <stdlib.h>

/* Static cache to store computed sequence values. */
static seq_t *cache = NULL;
static size_t cached_len = 0;

/* Ensure that cache contains terms up to index n. */
static void ensure_cache(idx_t n)
{
    /* Initial allocation: at least two slots to store a(0) and a(1). */
    if (!cache) {
        size_t init = 2;
        if ((size_t)n + 1 > init) init = (size_t)n + 1;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        cache[0] = (seq_t)1;
        if (init > 1) cache[1] = (seq_t)141;
        cached_len = init;
    }

    /* Grow cache if needed and compute missing terms. */
    if (cached_len <= (size_t)n) {
        size_t old_len = cached_len;
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

        idx_t start = (idx_t)old_len;
        for (idx_t i = start; i <= n; ++i) {
            /* a(i) = 142*a(i-1) - a(i-2) */
            cache[i] = (seq_t)142 * cache[i - 1] - cache[i - 2];
        }
        cached_len = new_len;
    }
}

/* Public interface: compute a(n). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
