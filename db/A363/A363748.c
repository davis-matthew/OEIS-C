/*
 * A363748  Number of compositions into sums of fourth powers.
 * https://oeis.org/A363748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1, k⁴≤n} a(n−k⁴); a(0)=1
 * Range:     u8 n <= 50, u16 n <= 92, u32 n <= 177, u64 n <= 346,
 *            u128 n <= 684, big any n
 * Validated: standard, pass, 98 terms through n = 9770 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below uses static memoisation.  It works for any
   seq_t type (built‑in integer, arbitrary‑precision integer, etc.)
   and for any idx_t type that can hold up to the requested index. */
#include <stddef.h>          /* for size_t */
#include "../utils/mem.h"

/* Required helper that allocates or expands a cache.  It constructs
   elements when seq_t is a class with a default constructor. */
#include <stdlib.h>

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
    static seq_t *dp = NULL;
    static idx_t computed = 0;   /* largest index computed so far */
    static size_t cap = 0;       /* current allocated capacity */

    /* Make sure the cache can hold indices up to n. */
    ensure_cache(n, &dp, &cap);

    /* Initialise a(0) if this is the first call. */
    if (computed == 0) {
        dp[0] = (seq_t)1;
        computed = 0;
    }

    /* Compute all values up to n that have not yet been cached. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t k = 1;
        while (1) {
            seq_t kk = (seq_t)k;
            seq_t kk2 = kk * kk;
            seq_t kk4 = kk2 * kk2;

            if (kk4 > (seq_t)i) break;          /* k⁴ > i → stop */
            idx_t j = i - (idx_t)kk4;           /* j = i − k⁴ */
            sum += dp[j];
            ++k;
        }
        dp[i] = sum;
    }

    computed = n;   /* update the highest computed index */
    return dp[n];
}
