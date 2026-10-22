/*
 * A310601  Coordination sequence Gal.6.32.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-8) - a(n-16) for n > 17 Initial values: a(0)=1,
 *            a(1)=4, a(2)=6, a(3)=14, a(4)=14, a(5)=18, a(6)=20, a(7)=30,
 *            a(8)=28, a(9)=38, a(10)=36, a(11)=46, a(12)=44, a(13)=48,
 *            a(14)=50, a(15)=62, a(16)=58, a(17)=70
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[18] = {
    (seq_t)1,  (seq_t)4,  (seq_t)6,  (seq_t)14, (seq_t)14,
    (seq_t)18, (seq_t)20, (seq_t)30, (seq_t)28, (seq_t)38,
    (seq_t)36, (seq_t)46, (seq_t)44, (seq_t)48, (seq_t)50,
    (seq_t)62, (seq_t)58, (seq_t)70
};

static seq_t *cache = NULL;      /* pointer to cached values */
static size_t cache_len = 0;     /* number of values currently cached */

/* Ensure that cache contains values up to index n (inclusive).
   The cache is allocated (or enlarged) as necessary. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_len)
        return;  /* already have enough */

    /* Determine new capacity: at least n+1, but grow in blocks */
    size_t new_cap = n + 1;
    if (!cache) {
        new_cap = (new_cap < 32) ? 32 : new_cap;
    } else if (new_cap <= cache_len) {
        /* already enough */
        return;
    }

    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – aborting by triggering a crash,
           as beyond this point the data is crucial. */
        abort();
    }
    cache = new_cache;

    /* If cache is newly allocated, fill the first 18 entries */
    if (cache_len < 18) {
        for (size_t i = cache_len; i < 18 && i < new_cap; ++i)
            cache[i] = init[i];
    }

    /* Compute further values if needed */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 18) {
            /* already filled from init */
            continue;
        }
        /* a(i) = 2*a(i-8) - a(i-16) */
        seq_t val = cache[i-8];
        val = val * ((seq_t)2);
        val = val - cache[i-16];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if ((size_t)n < 18) {
        return init[(size_t)n];
    }
    ensure_cache(n);
    return cache[(size_t)n];
}
