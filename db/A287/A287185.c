/*
 * A287185  a(n) = 3*a(n-2) + a(n-3) + a(n-5) - 3*a(n-6) - a(n-7), where a(0)
 *          = 2, a(1) = 4, a(2) = 7, a(3) = 14, a(4) = 25, a(5) = 47, a(6) =
 *          88, a(7) = 166.
 * https://oeis.org/A287185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + a(n-3) + a(n-5) - 3*a(n-7) - a(n-8), with
 *            a(0)=2, a(1)=4, a(2)=7, a(3)=14, a(4)=25, a(5)=47, a(6)=88,
 *            a(7)=166
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 69, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Lazy memoisation table for all previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cap    = 0;   /* allocated capacity of cache */
    static size_t last   = 0;   /* largest n already computed */
    static bool initialized = false;

    /* Ensure we have enough room for index n. */
    if (cache == NULL) {
        cap = (size_t)n + 1;
        if (cap < 8) cap = 8;                /* at least size of base data */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    } else if (cap <= (size_t)n) {
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Initialise base values only once.  They are needed for all n. */
    if (!initialized) {
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)25;
        cache[5] = (seq_t)47;
        cache[6] = (seq_t)88;
        cache[7] = (seq_t)166;
        /* The first five terms are defined in the statement; no further
           base values are required. */
        last = 7;
        initialized = true;
    }

    /* Return stored value if we have already computed it. */
    if ((size_t)n <= last) return cache[(size_t)n];

    /* Compute terms successively up to n. */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq_t val = (seq_t)3 * cache[i - 2]          /* 3*a(i-2) */
                    + cache[i - 3]                  /* + a(i-3) */
                    + cache[i - 5]                  /* + a(i-5) */
                    - (seq_t)3 * cache[i - 7]       /* -3*a(i-7) */
                    - cache[i - 8];                 /* - a(i-8) */
        cache[i] = val;
    }
    last = (size_t)n;
    return cache[(size_t)n];
}
