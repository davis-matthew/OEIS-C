/*
 * A254691  Number of length n+4 0..1 arrays with every five consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A254691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) + 3*a(n-5) * -
 *            3*a(n-6) - 2*a(n-8) + a(n-9) - 3*a(n-10) + a(n-11) + a(n-13) +
 *            a(n-15). * * Initial values (n = 1 .. 15): * 22, 34, 54, 86,
 *            136, 212, 334, 532, 852, 1367, * 2193, 3522, 5669, 9143, 14763
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Static cache and bookkeeping */
    static seq_t *cache = NULL;        /* array of a(0 .. max) (a(0) unused, a(1) first) */
    static size_t cache_cap = 0;       /* allocated size (remember a(0) unused) */
    static idx_t cache_last = 0;       /* last computed index */

    /* Grow the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;  /* need space for indices 0..n */
        /* seq_realloc may need a header, assume it is available */
        cache = seq_realloc(cache, cache_cap, new_cap);
        /* initialise newly allocated entries to zero */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Seed the initial 15 values if not already present */
    static const seq_t init_vals[16] = {
        /* 0 */ 0,
        /* 1 */ 22,  /* a(1)  */
        /* 2 */ 34,
        /* 3 */ 54,
        /* 4 */ 86,
        /* 5 */ 136,
        /* 6 */ 212,
        /* 7 */ 334,
        /* 8 */ 532,
        /* 9 */ 852,
        /*10 */ 1367,
        /*11 */ 2193,
        /*12 */ 3522,
        /*13 */ 5669,
        /*14 */ 9143,
        /*15 */ 14763
    };

    /* Compute all terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 15) {
            /* initialise first 15 terms */
            cache[i] = init_vals[i];
        } else {
            /* recurrence for i >= 16 */
            seq_t val = cache[i - 1];
            val = val + val;                  /* 2*a(i-1) */

            val = val - cache[i - 2];
            val = val + cache[i - 3];
            val = val - cache[i - 4];

            /* +3*a(i-5) */
            val = val + cache[i - 5];
            val = val + cache[i - 5];
            val = val + cache[i - 5];

            /* -3*a(i-6) */
            val = val - cache[i - 6];
            val = val - cache[i - 6];
            val = val - cache[i - 6];

            /* -2*a(i-8) */
            val = val - cache[i - 8];
            val = val - cache[i - 8];

            val = val + cache[i - 9];

            /* -3*a(i-10) */
            val = val - cache[i - 10];
            val = val - cache[i - 10];
            val = val - cache[i - 10];

            val = val + cache[i - 11];
            val = val + cache[i - 13];
            val = val + cache[i - 15];

            cache[i] = val;
        }
    }

    cache_last = n;
    return cache[n];
}
