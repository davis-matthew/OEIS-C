/*
 * A346119  Expansion of the e.g.f. sqrt(2*x*exp(x) - 2*exp(x) + 3).
 * https://oeis.org/A346119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-1) - Sum_{i=1}^{n-2} binomial(n-1,i) * a(i) * a(n-i)
 *            Derived from the e.g.f. identity y^2 = 2x e^x - 2e^x + 3 with
 *            y(0)=1
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 21, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* for malloc, realloc */

seq_t a(idx_t n) {
    /* static cache of previously computed terms */
    static seq_t *cache   = NULL;
    static idx_t  cacheSize = 0;   /* number of terms currently in cache (index of last term + 1) */

    if (n < cacheSize)            /* already computed */
        return cache[n];

    /* enlarge cache to hold terms up to n */
    size_t newCap = (size_t)n + 1;          /* n + 1 terms: indices 0 .. n */
    cache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
    if (!cache) {
        /* allocation failure – should not happen in test harness */
        abort();
    }

    /* compute terms from cacheSize up to n */
    for (idx_t k = cacheSize; k <= n; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)1;
        } else if (k == 1) {
            cache[1] = (seq_t)0;
        } else {
            idx_t m = k - 1;               /* works on n-1 in the recurrence */
            seq_t sum = 0;
            seq_t bin = 1;                 /* binomial coefficient C(m,0) -> will become C(m,1) first */

            /* loop i = 1 .. k-2  (i.e. 1 .. m-1) */
            for (idx_t i = 1; i <= k - 2; ++i) {
                /* update bin to C(m, i) */
                bin = (bin * (seq_t)(m - (i - 1))) / (seq_t)i;

                /* term = C(m,i) * a[i] * a[k-i] */
                seq_t term = cache[i];
                term *= cache[k - i];
                term *= bin;
                sum += term;
            }
            cache[k] = (seq_t)(k - 1) - sum;
        }
    }

    cacheSize = n + 1;
    return cache[n];
}
