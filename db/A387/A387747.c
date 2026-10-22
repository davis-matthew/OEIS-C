/*
 * A387747  a(n) = (1/4) * Sum_{k=0..n-1} binomial(6*n,6*k+3).
 * https://oeis.org/A387747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*Sum_{k=0..n-1} binomial(6*n,6*k+3) Recurrence: a(n)
 *            = 38*a(n-1) + 1691*a(n-2) - 1728*a(n-3) (n >= 5) Initial values:
 *            a(0)=0, a(1)=5, a(2)=110, a(3)=12563, a(4)=654764
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;        /* cached values a(0) .. a(cache_last) */
    static idx_t cache_last = 0;       /* highest index currently cached */

    /* Allocate or enlarge cache as needed */
    if (cache == NULL) {
        /* first call: allocate space for indices 0..n */
        size_t sz = (size_t)(n + 1);
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) abort();
    } else if (cache_last < n) {
        /* enlarge cache to fit indices up to n */
        size_t sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, sz * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Compute any missing values */
    for (size_t i = (size_t)(cache_last + 1); i <= (size_t)n; ++i) {
        idx_t idx = (idx_t)i;
        if (idx == 0) {
            cache[0] = (seq_t)0;
        } else if (idx == 1) {
            cache[1] = (seq_t)5;
        } else if (idx == 2) {
            cache[2] = (seq_t)110;
        } else if (idx == 3) {
            cache[3] = (seq_t)12563;
        } else if (idx == 4) {
            cache[4] = (seq_t)654764;
        } else {
            /* recurrence: a(n) = 38*a(n-1) + 1691*a(n-2) - 1728*a(n-3)  (n>=5) */
            cache[idx] = (seq_t)38 * cache[idx - 1] +
                         (seq_t)1691 * cache[idx - 2] -
                         (seq_t)1728 * cache[idx - 3];
        }
    }
    cache_last = n;
    return cache[n];
}
