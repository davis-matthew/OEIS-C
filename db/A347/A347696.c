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
 * Validated: standard, pass, 102 terms through n = 7969 (2026-11-03)
 * Helpers:   ilog2, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ilog2.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

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
        idx_t bitlen = ilog2(i);  /* highest set bit position */
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
