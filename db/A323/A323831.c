/*
 * A323831  a(0) = 5; thereafter a(n) is obtained by doubling a(n-1) and
 *          repeatedly deleting any string of identical digits.
 * https://oeis.org/A323831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 5; a(n) = repeatedly delete any run of identical
 *            consecutive digits from 2*a(n-1). (See OEIS A323831.)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   delete_runs_cascade, delete_runs_once
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/delete_runs_cascade.h"
#include "../utils/delete_runs_once.h"

/* Sequence accessor.  LRU-esque cache; works for any order of calls. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* If we already have the value, return it immediately. */
    if (cache_len > n) return cache[n];

    /* Initialize cache if empty. */
    if (cache_len == 0) {
        cache_cap = (n + 1 > 64) ? (n + 1) : 64;
        cache = (seq_t*)calloc((size_t)cache_cap, sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)5;
        cache_len = 1;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t doubled = prev * (seq_t)2;
        seq_t val = delete_runs_cascade(doubled);

        if (i >= cache_cap) {
            cache_cap = cache_cap * 2;
            cache = (seq_t*)realloc(cache, (size_t)cache_cap * sizeof(seq_t));
            if (!cache) exit(1);
        }
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
