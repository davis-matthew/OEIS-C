/*
 * A261957  Start with a single equilateral triangle for n=0; for the odd n-th
 *          generation add a triangle at each expandable side of the triangles
 *          of the (n-1)-th generation (this is the "vertex to side" version);
 *          for the even n-th generation use the "side to side" version; a(n)
 *          is the number of triangles add...
 * https://oeis.org/A261957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-8) - a(n-10) for n > 14 Initial values:
 *            a(0)=1, a(1)=3, a(2)=9, a(3)=12, a(4)=24, a(5)=12, a(6)=24,
 *            a(7)=18, a(8)=36, a(9)=33, a(10)=57, a(11)=45, a(12)=81,
 *            a(13)=36, a(14)=78
 * Range:     u8 n <= 41, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init_vals[15] = {
    1, 3, 9, 12, 24, 12, 24, 18, 36, 33,
    57, 45, 81, 36, 78
};

static seq_t *cache = NULL;          /* cache of computed terms          */
static size_t cache_cap = 0;         /* allocated capacity in cache       */
static size_t last_computed = (size_t)-1;  /* last index that holds a value   */

/* ensure that cache has space for indices up to 'needed-1' */
static void ensure_capacity(size_t needed) {
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute a(n) for any n >= 0, in any order of calls */
seq_t a(idx_t n) {
    /* lazy initialization of first 15 terms */
    if (!cache) {
        ensure_capacity(16);
        memcpy(cache, init_vals, sizeof(init_vals));
        /* fill the remaining entries with zeroes (unneeded but safe) */
        for (size_t i = 15; i < cache_cap; ++i) cache[i] = 0;
        last_computed = 14;
    }

    /* if n is within already computed range, return directly */
    if (n <= last_computed) {
        return cache[(size_t)n];
    }

    /* compute from last_computed+1 up to n inclusive */
    ensure_capacity((size_t)n + 1);
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        /* for i > 14 the recurrence holds */
        cache[i] = cache[i - 2] + cache[i - 8] - cache[i - 10];
    }
    last_computed = (size_t)n;
    return cache[(size_t)n];
}
