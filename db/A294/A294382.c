/*
 * A294382  Solution of the complementary equation a(n) = a(n-1)*b(n-2) - 1,
 *          where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A294382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = a(n-1)*b(n-2) - 1, with a(0)=1, a(1)=3, b(0)=2, b(1)=4.
 *            The complementary values satisfy b(k)=k+4 for k>=2. Thus for
 *            n>=4: a(n)=a(n-1)*(n+2)-1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;       /* capacity of cache array       */
    static idx_t  max_computed = 0;    /* highest index that has a value */

    /* ensure the cache can hold index n */
    if (n >= cache_cap) {
        /* the helper guarantees a new array big enough and updates cap */
        ensure_cache(n, &cache, &cache_cap);
    }

    /* initialise the known first four terms once */
    if (cache[0] == 0) { /* 0 used as sentinel; a(0)=1 is non‑zero */
        cache[0] = (seq_t)1ULL;   /* a(0) = 1 */
        cache[1] = (seq_t)3ULL;   /* a(1) = 3 */
        cache[2] = (seq_t)5ULL;   /* a(2) = 5 */
        cache[3] = (seq_t)19ULL;  /* a(3) = 19 */
        max_computed = 3;
    }

    /* already computed? */
    if (n <= max_computed)
        return cache[n];

    /* iterative filling up to n using a(n)=a(n‑1)*(n+2)-1 for n>=4 */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* i>=4, so formula applies directly */
        cache[i] = cache[i - 1] * ((seq_t)(i + 2)) - (seq_t)1ULL;
    }
    max_computed = n;
    return cache[n];
}
