/*
 * A332491  a(n) = 2*a(n-1) + a(n-3), where a(0) = 3, a(1) = 1, a(2) = 2.
 * https://oeis.org/A332491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-3) with a(0)=3, a(1)=1, a(2)=2
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoisation */
static seq_t *cache      = NULL;
static size_t cache_end = 0;   /* number of terms already computed (cache[0..cache_end-1]) */
static size_t cache_cap = 0;   /* allocated capacity of cache */

/* Ensure that cache has at least (n+1) elements computed. */
static void ensure(idx_t n)
{
    size_t needed = (size_t)n + 1;   /* indices 0..n inclusive */

    if (cache_end > n)                 /* already computed */
        return;

    /* allocation / capacity enlargement */
    if (cache == NULL) {
        /* initial allocation: at least first 3 terms */
        size_t init_cap = needed > 3 ? needed : 3;
        cache = (seq_t*)malloc(sizeof(seq_t) * init_cap);
        if (!cache) exit(EXIT_FAILURE);
        cache_cap      = init_cap;
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache_end    = 3; /* terms 0..2 are ready */
    } else if (cache_cap < needed) {
        size_t new_cap = cache_cap;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cache_end; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) + a(i-3) */
        cache[i] = (seq_t)((unsigned __int128)2 * cache[i-1] + cache[i-3]);
    }
    cache_end = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)3;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)2;

    ensure(n);
    return cache[(size_t)n];
}
