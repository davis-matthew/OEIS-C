/*
 * A335981  Expansion of e.g.f. exp(3 * (1 - exp(-x)) + x).
 * https://oeis.org/A335981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) + 3 * Σ_{k=0}^{n-1} (-1)^{n-k-1} * binomial(n-1,k)
 *            * a(k), a(0) = 1
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  size   = 0;   /* number of terms already computed */
    static idx_t  cap    = 0;   /* allocated capacity */

    /* initialize cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        cache[0] = 1;                 /* a(0) */
        size = 1;
        cap  = 1;
    }

    /* grow array if necessary */
    if (n + 1 > cap) {
        idx_t newcap = cap ? cap : 1;
        while (newcap <= n) newcap <<= 1;   /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0) continue; /* a(0) already set */

        seq_t sum   = 0;
        seq_t binom = 1;                          /* C(i-1,0) */

        for (idx_t k = 0; k < i; ++k) {
            /* term = a(k) * C(i-1,k) * (-1)^{i-k-1} */
            seq_t term = cache[k] * binom;

            /* sign = (-1)^(i-k-1) */
            if (((i - k - 1) & 1) != 0)
                term = -term;

            sum += term;

            /* update binom to C(i-1,k+1) for next iteration */
            if (k + 1 < i) {
                binom = binom * ( (seq_t)(i - 1 - k) );   /* multiply by (i-1-k) */
                binom = binom / ( (seq_t)(k + 1) );       /* divide by (k+1), exact */
            }
        }

        /* a(i) = a(i-1) + 3 * sum */
        cache[i] = cache[i - 1] + (sum * 3);
    }

    size = n + 1;          /* we now have terms up to n */
    return cache[n];
}
