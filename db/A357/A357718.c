/*
 * A357718  Expansion of e.g.f. cos( sqrt(3) * log(1+x) ).
 * https://oeis.org/A357718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(2*n-3)*a(n-1) - (n^2-4*n+7)*a(n-2) a(0) = 1, a(1) = 0
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size */
    static size_t sz    = 0;   /* number of values stored (>=0) */

    /* initialise cache on first call */
    if (!cache) {
        cap = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);          /* out of memory */
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)0;          /* a(1) */
        sz = 2;
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;              /* double until enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values up to n */
    if ((size_t)n + 1 > sz) {
        for (size_t k = sz; k <= (size_t)n; ++k) {
            /* helper variables */
            const idx_t kk = (idx_t)k;               /* promote loop index */
            const int64_t c1i = -(2 * (int64_t)kk - 3);
            const int64_t c2i = -( (int64_t)kk * (int64_t)kk
                                    - 4 * (int64_t)kk
                                    + 7 );
            const seq_t  coeff1 = (seq_t)c1i;
            const seq_t  coeff2 = (seq_t)c2i;

            /* recurrence */
            cache[k] = coeff1 * cache[k - 1] + coeff2 * cache[k - 2];
        }
        sz = (size_t)n + 1;
    }

    return cache[n];
}
