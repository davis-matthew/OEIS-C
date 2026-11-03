/*
 * A346074  a(n) = 1 + Sum_{k=0..n-5} a(k) * a(n-k-5).
 * https://oeis.org/A346074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 31, u32 n <= 57, u64 n <= 105,
 *            u128 n <= 201, big any n
 * Validated: standard, pass, 61 terms through n = 983 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

/*
 * Sequence A346074:  a(0)=a(1)=a(2)=a(3)=a(4)=1; for n>=5
 * a(n) = 1 + Σ_{k=0}^{n-5} a(k) * a(n-k-5).
 *
 * Memoisation with a static dynamic array allows the function to be
 * called in any order.  The helper `ensure_cache` (from the utils
 * library) guarantees a contiguous array of length at least n+1.
 */
seq_t a(idx_t n)
{
    /* static storage for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* current allocated size (in items) */
    static idx_t  computed = 0;   /* highest index whose value is known */

    /* make sure the cache can hold indices 0..n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise the five base values on the first call */
    if (computed == 0) {
        for (idx_t i = 0; i <= 4; ++i) {
            cache[i] = 1;
        }
        computed = 4;
    }

    /* already calculated? */
    if (n <= computed)
        return cache[n];

    /* compute all missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* guard; for i<=4 the recurrence is not used */
        if (i <= 4) {
            cache[i] = 1;
            continue;
        }

        seq_t sum = 0;
        for (idx_t k = 0; k <= i - 5; ++k) {
            sum += cache[k] * cache[i - k - 5];
        }
        cache[i] = sum + 1;
    }

    computed = n;
    return cache[n];
}
