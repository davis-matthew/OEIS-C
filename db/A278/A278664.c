/*
 * A278664  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 62", based on the 5-celled von
 *          Neumann neighborhood.
 * https://oeis.org/A278664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 10000*a(n-4) - 10000*a(n-5) for n>10 Initial
 *            values: a(0)=1, a(1)=11, a(2)=110, a(3)=1100, a(4)=11000,
 *            a(5)=110010, a(6)=1100110, a(7)=11000100, a(8)=110000100,
 *            a(9)=1100100100, a(10)=11001110100
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamic cache for computed values */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of values stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure cache capacity */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 11;
        if (new_cap <= n) new_cap = n + 1;
        if (new_cap < 11) new_cap = 11;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize first 11 terms if not already done */
    static const seq_t init[11] = {
        (seq_t)1ULL,          /* a(0) */
        (seq_t)11ULL,         /* a(1) */
        (seq_t)110ULL,        /* a(2) */
        (seq_t)1100ULL,       /* a(3) */
        (seq_t)11000ULL,      /* a(4) */
        (seq_t)110010ULL,     /* a(5) */
        (seq_t)1100110ULL,    /* a(6) */
        (seq_t)11000100ULL,   /* a(7) */
        (seq_t)110000100ULL,  /* a(8) */
        (seq_t)1100100100ULL, /* a(9) */
        (seq_t)11001110100ULL /* a(10) */
    };

    if (cache_len == 0) {
        size_t upto = (cache_cap < 11) ? cache_cap : 11;
        for (size_t i = 0; i < upto; ++i) cache[i] = init[i];
        cache_len = upto;
    } else if (cache_len < 11) {
        /* Fill missing initial terms if cache was resized after first use */
        for (size_t i = cache_len; i < 11; ++i) cache[i] = init[i];
        cache_len = 11;
    }

    /* Return value if already computed */
    if (n < cache_len) return cache[(size_t)n];

    /* Compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 11) continue; /* already set */
        seq_t term = cache[i-1]
                    + (cache[i-4] * (seq_t)10000)
                    - (cache[i-5] * (seq_t)10000);
        cache[i] = term;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
