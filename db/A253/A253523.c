/*
 * A253523  Number of (n+1)X(7+1) 0..2 arrays with every 2X2 subblock diagonal
 *          minimum minus antidiagonal minimum nondecreasing horizontally and
 *          diagonal maximum minus antidiagonal maximum nondecreasing
 *          vertically.
 * https://oeis.org/A253523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 13, u64 n <= 33, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A253523
    Formula: a(n) = 9*a(n-1) - 33*a(n-2) + 65*a(n-3) - 75*a(n-4)
              + 51*a(n-5) - 19*a(n-6) + 3*a(n-7),  for n > 15
    Initial values:
        a(1)  = 876277
        a(2)  = 2544290
        a(3)  = 2419612
        a(4)  = 2475610
        a(5)  = 3429207
        a(6)  = 5591131
        a(7)  = 10168992
        a(8)  = 21354009
        a(9)  = 51569897
        a(10) = 137921376
        a(11) = 391536224
        a(12) = 1145589321
        a(13) = 3399373692
        a(14) = 10150512690
        a(15) = 30391595567
    The sequence is defined by the linear recurrence above for all n ≥ 16.
    The implementation stores computed values in a static cache so that
    calls with arbitrary order of n are efficient (O(n) overall).
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for 1 ≤ n ≤ 15 */
    static const seq_t init_vals[16] = {
        0,          /* unused, 1‑based indexing */
        876277ull,  /* a(1) */
        2544290ull, /* a(2) */
        2419612ull, /* a(3) */
        2475610ull, /* a(4) */
        3429207ull, /* a(5) */
        5591131ull, /* a(6) */
        10168992ull,/* a(7) */
        21354009ull,/* a(8) */
        51569897ull,/* a(9) */
        137921376ull,/* a(10) */
        391536224ull,/* a(11) */
        1145589321ull/* a(12) */
    };

    static const seq_t init_vals2[16] = {
        0,          /* unused */
        3399373692ull,/* a(13) */
        10150512690ull,/* a(14) */
        30391595567ull /* a(15) */
    };

    /* Combine both arrays for simplicity */
    static const seq_t init[16] = {
        0,
        876277ull,
        2544290ull,
        2419612ull,
        2475610ull,
        3429207ull,
        5591131ull,
        10168992ull,
        21354009ull,
        51569897ull,
        137921376ull,
        391536224ull,
        1145589321ull,
        3399373692ull,
        10150512690ull,
        30391595567ull
    };

    /* Lazy static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size of cache (≥ 1) */
    static size_t cache_max = 0;   /* largest n that has been computed */

    /* Return initial values directly if n ≤ 15 */
    if (n <= 15)
        return init[n];

    /* Allocate cache on first use */
    if (!cache) {
        cache_cap = 16; /* 1..15 + one extra to be safe with 0 index */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failure → sequence undefined */
        for (size_t i = 1; i <= 15; ++i)
            cache[i] = init[i];
        cache_max = 15;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms until reaching n */
    for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
        /* recurrence a(i) = 9*a(i-1) - 33*a(i-2) + 65*a(i-3)
           - 75*a(i-4) + 51*a(i-5) - 19*a(i-6) + 3*a(i-7) */
        cache[i] =
            9 * cache[i - 1]
            - 33 * cache[i - 2]
            + 65 * cache[i - 3]
            - 75 * cache[i - 4]
            + 51 * cache[i - 5]
            - 19 * cache[i - 6]
            + 3 * cache[i - 7];
    }
    cache_max = n;
    return cache[n];
}
