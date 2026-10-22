/*
 * A352377  Expansion of e.g.f. exp(1 - cos(x) + sin(x)).
 * https://oeis.org/A352377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; a(n)=∑_{k=1..n} (-1)^⌊(k-1)/2⌋ * C(n-1,k-1) * a(n-k)
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 17, i64 n <= 28, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;      /* array of previously computed values      */
static size_t cache_len = 0;     /* number of values currently stored        */
static size_t cache_cap = 0;     /* allocated capacity of the array          */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
        cache[0] = 1;          /* a(0) = 1 */
        cache_len = 1;
    }

    /* extend cache to hold up to a(n) if needed */
    if (n >= cache_len) {
        /* increase capacity if necessary */
        if (cache_cap < (size_t)(n + 1)) {
            size_t new_cap = cache_cap;
            if (new_cap == 0) new_cap = 1;
            while (new_cap < (size_t)(n + 1))
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (size_t idx = cache_len; idx <= (size_t)n; ++idx) {
            idx_t m = (idx_t)(idx - 1);            /* m = n-1 */
            seq_t sum = 0;
            seq_t bin = 1;                         /* C(m,0) */

            for (idx_t j = 0; j <= m; ++j) {
                int sign = ((j >> 1) & 1) ? -1 : 1; /* (-1)^floor(j/2) */
                seq_t term = bin * cache[(size_t)m - (size_t)j];
                if (sign < 0)
                    term = -term;
                sum += term;

                if (j < m) {
                    bin = bin * (seq_t)(m - j);
                    bin = bin / (seq_t)(j + 1);   /* next binomial coefficient */
                }
            }

            cache[idx] = sum;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
