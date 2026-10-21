/*
 * A298921  Number of nX5 0..1 arrays with every element equal to 1, 3, 5, 7
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A298921:  Number of n×5 0..1 arrays with king‑move adjacency
 *
 *  Formula:  a(n) = a(n‑1) + a(n‑2) + a(n‑6) – a(n‑7) – a(n‑8)      for n > 11
 *  Initial values:
 *      a(1)=0,  a(2)=32,  a(3)=3,  a(4)=7,  a(5)=9,
 *      a(6)=12, a(7)=17, a(8)=41, a(9)=42, a(10)=67,
 *      a(11)=109, a(12)=172
 *
 *  The function caches computed terms so that calls in any order run in O(n)
 *  time and O(n) memory.
 *
 *  The types seq_t and idx_t are defined in the build environment.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached terms, 1‑based indexing, cache[0] unused   */
static idx_t  cache_cap = 0;  /* current allocation size (>= cache_n+1)          */
static idx_t  cache_n   = 0;  /* highest index that has been computed           */

static const seq_t init_vals[12] = {
    0, 32, 3, 7, 9, 12,
    17, 41, 42, 67, 109, 172
};

/* Ensure cache has space for indices up to need (inclusive). */
static void
ensure_cache(idx_t need)
{
    if (cache == NULL) {
        cache_cap = need + 1;
        if (cache_cap < 13) cache_cap = 13;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    } else if (need + 1 > cache_cap) {
        cache_cap = need + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    }
    if (cache_n == 0) {                /* first touch: initialise a(1)..a(12) */
        for (idx_t i = 1; i <= 12; ++i)
            cache[i] = init_vals[i-1];
        cache_n = 12;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */

    /* Return known small terms directly */
    if (n <= 12) return init_vals[n-1];

    /* Grow cache if needed and compute missing terms */
    ensure_cache(n);
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        /* All indexes i-1 … i-8 are valid because i >= 13. */
        cache[i] = cache[i-1] + cache[i-2];
        cache[i] += cache[i-6];
        cache[i] -= cache[i-7];
        cache[i] -= cache[i-8];
    }
    cache_n = n;
    return cache[n];
}
