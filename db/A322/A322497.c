/*
 * A322497  Number of tilings of a 4 X n rectangle using V (2m+1)-ominoes (m
 *          >= 0) in standard orientation.
 * https://oeis.org/A322497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sequence A322497 Recurrence (n ≥ 8): a(n) = a(n-1) + 5·a(n-2) +
 *            9·a(n-3) + 17·a(n-4) + 13·a(n-5) + 5·a(n-6) - 2·a(n-7) -
 *            2·a(n-8) Initial values: a(0)=1, a(1)=1, a(2)=5, a(3)=18,
 *            a(4)=68, a(5)=233, a(6)=838, a(7)=2989
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 954 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;
    /* first eight terms */
    const seq_t base[8] = {
        (seq_t)1,
        (seq_t)1,
        (seq_t)5,
        (seq_t)18,
        (seq_t)68,
        (seq_t)233,
        (seq_t)838,
        (seq_t)2989
    };

    if (n < 8)               /* directly return a given term */
        return base[(size_t)n];

    ensure_cache(n + 1, &cache, &cap);   /* guarantees cache has room for indices 0..n */

    /* initialise cache with the known first 8 values, if not already done */
    if (size < 8)
    {
        for (size_t i = size; i < 8; ++i)
            cache[i] = base[i];
        size = 8;
    }

    /* compute terms on demand up to n */
    for (size_t i = size; i <= (size_t)n; ++i)
    {
        seq_t val = cache[i-1]
                  + (seq_t)5 * cache[i-2]
                  + (seq_t)9 * cache[i-3]
                  + (seq_t)17 * cache[i-4]
                  + (seq_t)13 * cache[i-5]
                  + (seq_t)5 * cache[i-6]
                  - (seq_t)2 * cache[i-7]
                  - (seq_t)2 * cache[i-8];
        cache[i] = val;
    }
    size = (size_t)n + 1;

    return cache[(size_t)n];
}
