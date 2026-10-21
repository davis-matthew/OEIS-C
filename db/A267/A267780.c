/*
 * A267780  Decimal representation of the n-th iteration of the "Rule 211"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 4*a(n-3) for n > 3 Initial values:
 *            a(0) = 1, a(1) = 5, a(2) = 25, a(3) = 125
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* dynamically allocated array of terms   */
    static size_t cache_cap = 0;     /* current capacity of cache              */
    static size_t computed_up_to = 0;/* highest index already computed         */
    static int initialized = 0;      /* flag to initialise base terms once      */

    /* ----------------------------------------------------------- */
    /* 1. Initialise base terms the first time we are called.    */
    /* ----------------------------------------------------------- */
    if (!initialized) {
        /* Allocate space for the first four terms. */
        cache_cap = 4;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);     /* allocation failure */

        /* Store the base values. */
        cache[0] = 1;
        cache[1] = 5;
        cache[2] = 25;
        cache[3] = 125;

        computed_up_to = 3;
        initialized = 1;
    }

    /* ----------------------------------------------------------- */
    /* 2. If n is within the already computed range, return it.  */
    /* ----------------------------------------------------------- */
    if (n <= computed_up_to) {
        return cache[n];
    }

    /* ----------------------------------------------------------- */
    /* 3. Ensure the cache has enough space to hold a(n).         */
    /* ----------------------------------------------------------- */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ----------------------------------------------------------- */
    /* 4. Compute missing terms up to n (inclusive).               */
    /* ----------------------------------------------------------- */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        /* recurrence: a(i) = 4*a(i-1) + a(i-2) - 4*a(i-3) */
        cache[i] = 4 * cache[i - 1] + cache[i - 2] - 4 * cache[i - 3];
    }
    computed_up_to = n;

    /* ----------------------------------------------------------- */
    /* 5. Return the requested term.                              */
    /* ----------------------------------------------------------- */
    return cache[n];
}
