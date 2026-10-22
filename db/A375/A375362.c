/*
 * A375362  Expansion of 1/( (1 + x) * (1 - x*(1 + x)^3) ).
 * https://oeis.org/A375362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-2) + 6*a(n-3) + 4*a(n-4) + a(n-5) with initial
 *            values a(0)=1, a(1)=0, a(2)=4, a(3)=6, a(4)=20
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;        /* allocated slots */
    static idx_t  last = 0;             /* largest index stored */

    /* Helper macro for accessing cache safely */
    #define CACHE(i) (cache[(i)])

    /* Initialize cache with base terms if needed */
    if (!cache) {
        cache_cap = 5;                  /* we need indices 0..4 at least */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure; return 0 as safe default */

        CACHE(0) = (seq_t)1;
        CACHE(1) = (seq_t)0;
        CACHE(2) = (seq_t)4;
        CACHE(3) = (seq_t)6;
        CACHE(4) = (seq_t)20;
        last = 4;
    }

    /* If the requested index is already cached, return it */
    if (n <= last) {
        return CACHE(n);
    }

    /* Ensure sufficient capacity */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* The recurrence is only valid for i >= 5; for i < 5 we already have values */
        if (i < 5) {
            /* This branch is never reached because last >= 4 before and we stop at n>=5 */
            continue;
        }
        CACHE(i) = (seq_t)4 * CACHE(i - 2)
                + (seq_t)6 * CACHE(i - 3)
                + (seq_t)4 * CACHE(i - 4)
                +       CACHE(i - 5);
    }

    last = n;
    return CACHE(n);
}
