/*
 * A280174  Number of n X 2 0..1 arrays with no element unequal to a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors,
 *          with the exception of exactly two elements, and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A280174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 53, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A280174 – recurrence implementation
 *
 *  Empirical recurrence for n > 14:
 *      a(n) = 4*a(n-1) - 11*a(n-3) - 4*a(n-4) + 15*a(n-5) + 14*a(n-6)
 *           - 2*a(n-7) - 9*a(n-8) - 5*a(n-9) - a(n-10)
 *  Initial values a(1)..a(14) are supplied below.
 */

#include <stddef.h>
#include <stdlib.h>

/* Initial terms a(1) .. a(14) for the recurrence */
static const seq_t init[15] = {
    0,          /* a(0) unused */
    1,          /* a(1)  */
    2,          /* a(2)  */
    12,         /* a(3)  */
    41,         /* a(4)  */
    103,        /* a(5)  */
    263,        /* a(6)  */
    656,        /* a(7)  */
    1618,       /* a(8)  */
    3931,       /* a(9)  */
    9459,       /* a(10) */
    22557,      /* a(11) */
    53395,      /* a(12) */
    125594,     /* a(13) */
    293796      /* a(14) */
};

seq_t a(idx_t n)
{
    /* Cache to store already computed values.  Indices 1..cap are used. */
    static seq_t *cache      = NULL;   /* dynamic array for a(1)..a(cap)   */
    static idx_t  cap        = 0;      /* allocated capacity (max index)   */
    static idx_t  max_comp   = 0;      /* highest index computed so far    */

    /* Return the initial value directly for small n. */
    if (n <= 14) {
        return init[(idx_t)n];
    }

    /* Grow the cache if necessary. */
    if (cap < n) {
        idx_t new_cap = n * 2;
        if (new_cap < n + 128) {
            new_cap = n + 128;   /* safety margin for small increments */
        }
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 as a fallback. */
            return 0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise cache[1..14] if not already done. */
    if (max_comp < 14) {
        for (idx_t i = 1; i <= 14; ++i) {
            cache[i] = init[i];
        }
        max_comp = 14;
    }

    /* If already computed up to n, just return the value. */
    if (n <= max_comp) {
        return cache[n];
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t val = cache[i-1] * 4
                  - cache[i-3] * 11
                  - cache[i-4] * 4
                  + cache[i-5] * 15
                  + cache[i-6] * 14
                  - cache[i-7] * 2
                  - cache[i-8] * 9
                  - cache[i-9] * 5
                  - cache[i-10];
        cache[i] = val;
    }

    max_comp = n;
    return cache[n];
}
