/*
 * A347696  Length of longest sequence of directed edges in the graph G (see
 *          Comments) that starts at node n.
 * https://oeis.org/A347696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of longest sequence of edges in graph G defined in
 *            the OEIS entry
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   ensure_cache, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"
#include "../utils/floor_log2_uint64.h"

static seq_t *cache_ptr   = NULL; /* cache of computed values a(0..cache_max) */
static size_t cache_cap   = 0;    /* capacity of cache_ptr */
static idx_t  cache_max   = 0;    /* highest index computed so far */

/* main function */
seq_t a(idx_t n)
{
    /* lazy initialization of cache[0] */
    if (cache_ptr == NULL) {
        ensure_cache(0, &cache_ptr, &cache_cap);
        cache_ptr[0] = (seq_t)0;
        cache_max = 0;
    }

    if (n == 0) return cache_ptr[0];
    if (n <= cache_max) return cache_ptr[n];

    /* extend cache up to index n */
    ensure_cache(n, &cache_ptr, &cache_cap);
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        idx_t bitlen = floor_log2_uint64(i);  /* highest set bit position */
        seq_t best = 0;
        for (idx_t s = 0; s <= bitlen; ++s) {
            if ((((i >> s) & (idx_t)1) == 0)) {    /* zero bit at position s */
                idx_t target = i - ((idx_t)1 << s);
                seq_t cand = cache_ptr[target] + (seq_t)1;
                if (cand > best) best = cand;
            }
        }
        cache_ptr[i] = best;
    }
    cache_max = n;
    return cache_ptr[n];
}
