/*
 * A281749  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 395", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A281749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -10*a(n-1) + 1000*a(n-3) + 10000*a(n-4) + a(n-8) +
 *            10*a(n-9) - 1000*a(n-11) - 10000*a(n-12) for n > 19 Initial
 *            values: a(0) = 1 a(1) = 10 a(2) = 111 a(3) = 10 a(4) = 10111
 *            a(5) = 111010 a(6) = 10111 a(7) = 10101010 a(8) = 111010111 a(9)
 *            = 10111010 a(10) = 10101010111 a(11) = 111011101010 a...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   str_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/str_to_seq.h"

/* Helper to convert a decimal string (containing only '0'..'9') to seq_t */

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last  = 0;

    /* String representations of the first 20 terms */
    static const char *base_str[20] = {
        "1", "10", "111", "10", "10111",
        "111010", "10111", "10101010", "111010111", "10111010",
        "10101010111", "111011101010", "10101010111", "10101110111010",
        "111010101010111", "10111011101010", "10101010101010111",
        "111011101010111010", "10101010101010111", "10101110101011101010"
    };

    /* Allocate and initialise the cache if this is the first call */
    if (cache == NULL) {
        cap = 20;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 20; ++i)
            cache[i] = str_to_seq(base_str[i]);
        last = 19;
    }

    /* Ensure the cache can hold up to index n */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)n + 1)
            newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* If the requested index is already computed, return it */
    if (n <= last)
        return cache[(size_t)n];

    /* Compute missing terms up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        if (i <= 19) /* already initialised */
            continue;
        /* Compute according to the linear recurrence */
        seq_t term1 = cache[i-3] * 1000;
        term1 += cache[i-4] * 10000;
        term1 += cache[i-8];
        term1 += cache[i-9] * 10;

        seq_t term2 = cache[i-1] * 10;
        term2 += cache[i-11] * 1000;
        term2 += cache[i-12] * 10000;

        cache[i] = term1 - term2;
    }
    last = n;
    return cache[(size_t)n];
}
