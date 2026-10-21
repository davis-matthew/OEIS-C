/*
 * A298802  Growth series for group with presentation < S, T : S^4 = T^4 =
 *          (S*T)^4 = 1 >.
 * https://oeis.org/A298802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) - a(n-4) for n>4 with initial values
 *            a(0)=1, a(1)=4, a(2)=10, a(3)=24, a(4)=56
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached terms */
static idx_t cache_size = 0;  /* number of valid entries in cache */
static idx_t cache_cap  = 0;  /* allocated capacity */

/* Extend the cache to cover index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* If we already have terms up to n, nothing to do. */
    if (cache_size > n) return;

    /* First time: allocate and initialise base values. */
    if (cache == NULL) {
        cache_cap = 8;                         /* initial capacity */
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();            /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)24;
        cache[4] = (seq_t)56;
        cache_size = 5;                       /* a(0)…a(4) ready */
    }

    /* Grow the allocated array if needed. */
    if (n >= cache_cap) {
        while (cache_cap <= n) cache_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (new_cache == NULL) abort();
        cache = new_cache;
    }

    /* Compute new terms only when necessary. */
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = (seq_t)2 * cache[i - 1];
            val += (seq_t)2 * cache[i - 3];
            val -= cache[i - 4];
            cache[i] = val;
        }
        /* Keep cache_size at the largest index computed so far. */
        if (n + 1 > cache_size) cache_size = n + 1;
    }
}

/* Public API */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
