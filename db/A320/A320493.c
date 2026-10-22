/*
 * A320493  Coordination sequence of thinnest 5-neighbor packing of the plane
 *          with congruent triangles with respect to a hexavalent point with
 *          no symmetry.
 * https://oeis.org/A320493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-3) + a(n-5) – a(n-8) for n > 8,
 * with initial values: a(0)=1, a(1)=6, a(2)=10, a(3)=17,
 * a(4)=23, a(5)=26, a(6)=35, a(7)=39, a(8)=43
 */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* memoized values */
static size_t cache_cap = 0;         /* allocated size of cache */
static size_t cache_len = 0;         /* number of computed entries */

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_n(idx_t n) {
    /* Convert to size_t for array indexing (n <= 10000 as per requirements). */
    size_t N = (size_t)n;

    /* If cache is not yet allocated, allocate minimal size needed for base values. */
    if (cache == NULL) {
        size_t needed = N + 1;
        if (needed < 9) needed = 9;          /* need base values up to a(8) */
        cache_cap = needed;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;                  /* allocation failure – caller will see 0 */
        /* initialise to zero */
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        /* base values */
        cache[0] = 1; cache[1] = 6; cache[2] = 10;
        cache[3] = 17; cache[4] = 23; cache[5] = 26;
        cache[6] = 35; cache[7] = 39; cache[8] = 43;
        cache_len = 9;
    }
    /* Grow cache if necessary */
    else if (N + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < N + 1) new_cap = N + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                      /* reallocation failure – caller will see 0 */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to N using the recurrence */
    while (cache_len <= N) {
        size_t i = cache_len;                 /* new index to compute */
        /* i ≥ 9 because cache_len starts at 9 and increments */
        cache[i] = cache[i - 3] + cache[i - 5] - cache[i - 8];
        ++cache_len;
    }
}

/* Public interface expected by the build system. */
seq_t a(idx_t n) {
    ensure_n(n);
    return cache[(size_t)n];
}
