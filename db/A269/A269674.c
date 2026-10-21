/*
 * A269674  Number of length-n 0..4 arrays with no repeated value differing
 *          from the previous repeated value by other than plus or minus one
 *          modulo 4+1.
 * https://oeis.org/A269674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) - 6*a(n-2) - 24*a(n-3) for n > 3
   Initial values: a(1) = 5, a(2) = 25, a(3) = 120
   The function is memoised and works for non‑increasing calls. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* For indices below the offset the value is irrelevant. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Static cache: cache[1..cap-1] hold computed terms. */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;   /* number of slots allocated (cache size) */

    if (cache == NULL) {                          /* first call: allocate */
        cap = (idx_t)((size_t)n + 1U);
        if (cap < 4U) {                          /* ensure room for 1..3 */
            cap = 4U;
        }
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) {                              /* allocation failed */
            return (seq_t)0;
        }
        /* Initialise base terms (up to the requested index). */
        if (cap > 1U) cache[1] = (seq_t)5;
        if (cap > 2U) cache[2] = (seq_t)25;
        if (cap > 3U) cache[3] = (seq_t)120;
        /* Compute remaining terms up to cap-1. */
        for (idx_t i = 4U; i < cap; ++i) {
            cache[i] = (seq_t)(7 * cache[i - 1]
                              - 6 * cache[i - 2]
                              - 24 * cache[i - 3]);
        }
    } else if (n >= cap) {                         /* grow the cache */
        idx_t new_cap = (idx_t)((size_t)n + 1U);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                               /* reallocation failed */
            return (seq_t)0;
        }
        cache = tmp;
        /* Initialise new slots if they were not set yet. */
        if (cap <= 1U && new_cap > 1U) cache[1] = (seq_t)5;
        if (cap <= 2U && new_cap > 2U) cache[2] = (seq_t)25;
        if (cap <= 3U && new_cap > 3U) cache[3] = (seq_t)120;
        /* Compute terms from old cap up to new_cap-1. */
        for (idx_t i = cap; i < new_cap; ++i) {
            cache[i] = (seq_t)(7 * cache[i - 1]
                              - 6 * cache[i - 2]
                              - 24 * cache[i - 3]);
        }
        cap = new_cap;
    }

    return cache[n];
}
