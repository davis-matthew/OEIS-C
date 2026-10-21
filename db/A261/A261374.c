/*
 * A261374  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00001011 00010101 or 01010101.
 * https://oeis.org/A261374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 33, u64 n <= 71, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + 6*a(n-4) + 4*a(n-6) - a(n-8)
   Initial values: a(1)=39, a(2)=41, a(3)=82, a(4)=157, a(5)=266,
   a(6)=470, a(7)=864, a(8)=1553, a(9)=2758 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, harmless */

    /* Static cache: 1‑based indexing; index 0 unused. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;         /* allocated size (bytes) */
    static idx_t  last_computed = 0;     /* highest index computed so far */

    /* Initial values for n = 1 … 9. Stored as 1‑based array. */
    static const seq_t init_vals[10] = {
        0ULL,   /* dummy for index 0 */
        39ULL, 41ULL, 82ULL, 157ULL, 266ULL,
        470ULL, 864ULL, 1553ULL, 2758ULL
    };

    /* Resize cache if the requested index exceeds current capacity. */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* abort on allocation failure */
        if (tmp != cache) {               /* realloc may move the block */
            cache = tmp;
        }
        /* Initialise newly allocated portion to zero. */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        size_t ii = (size_t)i;
        if (i <= 9) {
            cache[ii] = init_vals[i];
        } else {
            seq_t v2 = cache[ii - 2];
            seq_t v4 = cache[ii - 4] * 6;
            seq_t v6 = cache[ii - 6] * 4;
            seq_t v8 = cache[ii - 8];
            cache[ii] = v2 + v4 + v6 - v8;
        }
    }
    last_computed = n;
    return cache[n];
}
