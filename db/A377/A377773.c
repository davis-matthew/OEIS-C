/*
 * A377773  Number of edge cuts in the n-web graph.
 * https://oeis.org/A377773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16^n - 8^n + A359621(n) * Recurrence: a(n) = 28*a(n-1) -
 *            242*a(n-2) + 888*a(n-3) - 1481*a(n-4) * + 1196*a(n-5) -
 *            452*a(n-6) + 64*a(n-7) * Initial values: a(0)=1, a(1)=12,
 *            a(2)=223, a(3)=3898, a(4)=64453, * a(5)=1042868, a(6)=16747743
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* array holding computed values */
static size_t cache_cap = 0;          /* capacity of the cache array */
static idx_t  cached_max = (idx_t)-1; /* highest index computed so far */

/* Ensure the cache array can hold indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;

    size_t new_cap = (size_t)(n + 1);
    if (!cache) {
        cache = seq_alloc(new_cap);
    } else {
        cache = seq_realloc(cache, cache_cap, new_cap);
    }
    cache_cap = new_cap;
}

/* Fill the first seven terms of the sequence. */
static void init_base_values(void)
{
    ensure_cache_size(6);  /* need space for indices 0..6 */
    static const seq_t base[7] = {
        (seq_t)1,
        (seq_t)12,
        (seq_t)223,
        (seq_t)3898,
        (seq_t)64453,
        (seq_t)1042868,
        (seq_t)16747743
    };
    for (idx_t i = 0; i < 7; ++i)
        cache[i] = base[i];
    cached_max = 6;
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);

    /* On first use, seed the cache with the base values. */
    if (cached_max == (idx_t)-1)
        init_base_values();

    /* If we already know a(n), return it. */
    if (n <= cached_max)
        return cache[n];

    /* Compute missing terms by the 7‑term linear recurrence. */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t val = 28  * cache[i-1]
                  - 242 * cache[i-2]
                  + 888 * cache[i-3]
                  - 1481* cache[i-4]
                  + 1196* cache[i-5]
                  - 452 * cache[i-6]
                  + 64  * cache[i-7];
        cache[i] = val;
    }
    cached_max = n;
    return cache[n];
}
