/*
 * A334570  E.g.f.: exp(-(x + x^2/2 + x^3/3 + x^4/4)).
 * https://oeis.org/A334570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - (n-1)! * Σ_{k=1..min(4,n)} a(n-k)/(n-k)! (a(0)=1)
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 16, i64 n <= 25, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             rewritten to avoid fractions:                     */
/*             a(n) = - Σ_{k=1..min(4,n)} a(n-k) * Π_{t=n-k+1}^{n-1} t     */

#include <stddef.h>
#include <stdlib.h>

/* The function uses static memory so that repeated calls are fast
   and the sequence is memoised.  It works for any seq_t type supplied
   by the build (int*_t, __int128, or an arbitrary‑precision class). */
seq_t a(idx_t n)
{
    /* Static cache of already computed values. */
    static seq_t *cache   = NULL;   /* array of a(i) values            */
    static size_t  cap    = 0;      /* allocated capacity of the array */
    static size_t  last   = 0;      /* highest index that has been computed */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cap   = 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failed: give up */
        cache[0] = 1;                  /* a(0) = 1 */
        last = 0;                      /* only index 0 computed so far */
    }

    /* Grow the cache if necessary for the requested index. */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;  /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute any missing entries up to n. */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        /* k runs from 1 to min(4,i). */
        size_t kmax = i < 4 ? i : 4;
        seq_t sum = 0;
        for (size_t k = 1; k <= kmax; ++k) {
            /* product = Π_{t=i-k+1}^{i-1} t.
               The loop runs at most three iterations. */
            seq_t prod = 1;
            for (size_t t = i - k + 1; t < i; ++t) {
                prod *= (seq_t)t;
            }
            sum += cache[i - k] * prod;
        }
        cache[i] = -sum;
    }

    last = (size_t)n;            /* update the last computed index */
    return cache[n];
}
