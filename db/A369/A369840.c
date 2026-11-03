/*
 * A369840  Number of compositions of 5*n into parts 2 and 5.
 * https://oeis.org/A369840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) with
 *            initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=7, a(4)=23
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 981 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
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
#include <stddef.h>

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_n = 0;

    if (!initialized)
    {
        /* Allocate space for the first 5 terms. */
        ensure_cache(5, &cache, &cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)23;
        initialized = 1;
        max_n = 4;
    }

    if (n <= max_n)
        return cache[n];

    /* Ensure we have space for up to index n. */
    if (n >= (idx_t)cap)
        ensure_cache((idx_t)(n + 1), &cache, &cap);

    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        seq_t val = 5 * cache[i - 1];
        val -= 9 * cache[i - 2];
        val += 10 * cache[i - 3];
        val -= 5 * cache[i - 4];
        val += cache[i - 5];
        cache[i] = val;
    }

    max_n = n;
    return cache[n];
}
