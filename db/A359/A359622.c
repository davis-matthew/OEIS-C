/*
 * A359622  Number of edge cuts in the n-Moebius ladder.
 * https://oeis.org/A359622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A359622: Number of edge cuts in the n‑Moebius ladder.
   Recurrence (derived from the generating function):
       a(n) = 20*a(n-1) -146*a(n-2) +488*a(n-3) -777*a(n-4)
            +612*a(n-5) -228*a(n-6) +32*a(n-7)   for n ≥ 8
   Initial terms (offset 1):
       a(1)=1, a(2)=26, a(3)=307, a(4)=3004, a(5)=27049,
       a(6)=232658, a(7)=1947103 */
#include <stdlib.h>

static seq_t *cache = NULL;       /* memoization array, cache[0] unused */
static idx_t  max_cached = 0;     /* highest n written into cache */

/* Helper to grow the cache to at least size (n+1). */
static void ensure_cache_size(idx_t n)
{
    if (n <= max_cached)
        return;

    size_t needed = (size_t)n + 1;
    if (cache == NULL) {
        /* Allocate initial cache for indices 0..7 */
        size_t cap = 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) { /* allocation failed – abort by returning 0 */
            max_cached = 0;
            return;
        }
        for (size_t i = 0; i < cap; ++i)
            cache[i] = 0;

        /* Set the known initial values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)26;
        cache[3] = (seq_t)307;
        cache[4] = (seq_t)3004;
        cache[5] = (seq_t)27049;
        cache[6] = (seq_t)232658;
        cache[7] = (seq_t)1947103;
        cap = 8;
        max_cached = 7;

        if (needed <= cap)
            return;   /* all needed values already present */
    }

    /* Grow the cache if necessary */
    size_t cap = (size_t)max_cached + 1;
    if (needed > cap) {
        size_t new_cap = needed * 2;   /* simple growth factor */
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)               /* allocation failed */
            return;
        /* Initialize newly allocated slots to 0 */
        for (size_t i = cap; i < new_cap; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t t = cache[i-1] * (seq_t)20;
        t = t - cache[i-2] * (seq_t)146;
        t = t + cache[i-3] * (seq_t)488;
        t = t - cache[i-4] * (seq_t)777;
        t = t + cache[i-5] * (seq_t)612;
        t = t - cache[i-6] * (seq_t)228;
        t = t + cache[i-7] * (seq_t)32;
        cache[i] = t;
    }
    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* offset 1: a(1) is the first term */
        return (seq_t)0;

    ensure_cache_size(n);
    return cache[n];
}
