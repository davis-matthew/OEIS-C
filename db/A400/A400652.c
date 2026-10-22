/*
 * A400652  a(0) = 1; a(n) = a(n-1)*a(floor(n/2)) + n for n >= 1.
 * https://oeis.org/A400652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(0) = 1; a(n) = a(n-1) * a(floor(n/2)) + n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Computes A400652:  a(0)=1, a(n)=a(n-1)*a(floor(n/2))+n  (n≥1) */
seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;  /* memoization array */
    static size_t  cap      = 0;      /* allocated capacity */
    static idx_t   computed_up_to = 0; /* largest n already computed */
    static bool    initialized = false;

    /* Ensure we have room for indices up to n */
    if (cap < (size_t)(n + 1)) {
        size_t newcap = cap ? cap * 2 : 4;
        while (newcap < (size_t)(n + 1))
            newcap <<= 1; /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory: abort (alternatively, return 0) */
            abort();
        }
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise a(0) on first use */
    if (!initialized) {
        cache[0] = (seq_t)1;
        computed_up_to = 0;
        initialized = true;
    }

    /* If the requested value is already computed, return it */
    if (n <= computed_up_to)
        return cache[n];

    /* Compute values up to n iteratively */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * cache[i / 2] + (seq_t)i;
    }
    computed_up_to = n;
    return cache[n];
}
