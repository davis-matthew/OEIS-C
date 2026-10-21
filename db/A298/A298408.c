/*
 * A298408  a(n) = 2*a(n-1) - a(n-3) + 2*a(floor(n/2)) + 3*a(floor(n/3)) + ...
 *          + n*a(floor(n/n)), where a(0) = 1, a(1) = 1, a(2) = 1.
 * https://oeis.org/A298408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + 2*a(floor(n/2)) + 3*a(floor(n/3)) +
 *            ... + n*a(floor(n/n)), a(0) = 1, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 38, u64 n <= 84, u128 n <= 176,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
/* The utilities provided by the framework are already compiled */
#include "../utils/ensure_cache.h"

/* The sequence type and index type are supplied by the build */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t   upto   = 0;            /* largest index computed so far */

    /* Ensure we have room for at least n+1 elements (indices 0..n) */
    ensure_cache(n + 1, &cache, &cap);

    /* If nothing computed yet, initialise the first three values */
    if (upto == 0) {
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        upto = 2;
    }

    /* Compute all missing values up to n */
    while (upto < n) {
        idx_t i = ++upto;          /* next index to compute */

        seq_t val = 0;

        /* 2*a(i-1) - a(i-3) */
        val = 2 * cache[i-1];
        val -= cache[i-3];

        /* Sum j*a(floor(i/j)) for j = 2 .. i */
        for (idx_t j = 2; j <= i; ++j) {
            idx_t f = i / j;
            val += (seq_t)j * cache[f];
        }

        cache[i] = val;
    }

    return cache[n];
}
