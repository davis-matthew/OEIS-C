/*
 * A355414  Expansion of the e.g.f. log((1 - x) / (1 - 2*x)) / (1 - x)^5.
 * https://oeis.org/A355414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * The sequence satisfies the order‑3 recurrence * a(n) =
 *            (4n+5)*a(n-1) - (n+3)*(5n-3)*a(n-2) + 2*(n-2)*(n+3)*(n+2)*a(n-3)
 *            * for n ≥ 3, * with initial values a(0)=0, a(1)=1, a(2)=13. *
 *            This implementation memoises computed terms so any order of
 *            calls * is supported efficiently
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache if this is the first call */
    if (cache == NULL) {
        /* ensure room for at least the first three values or up to n */
        cache_len = (n + 1 > 3) ? n + 1 : 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;   /* memory failure → zero */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 13;
    }

    /* if we already have the requested term, just return it */
    if (n < cache_len)
        return cache[n];

    /* grow the cache to at least n+1 entries */
    idx_t old_len = cache_len;
    cache_len = n + 1;
    cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    if (cache == NULL) return 0;   /* memory failure → zero */

    /* compute missing terms from old_len up to n */
    for (idx_t i = old_len; i <= n; ++i) {
        if (i == 0)          cache[i] = 0;
        else if (i == 1)     cache[i] = 1;
        else if (i == 2)     cache[i] = 13;
        else {
            seq_t b = cache[i-1];
            seq_t c = cache[i-2];
            seq_t d = cache[i-3];

            /* (4i+5)*b */
            seq_t term1 = b * (seq_t)(4*i + 5);

            /* 2*(i-2)*(i+3)*(i+2)*d */
            seq_t term3 = d * (seq_t)(2*(i-2)*(i+3)*(i+2));

            /* (i+3)*(5i-3)*c */
            seq_t term2 = c * (seq_t)((i+3)*(5*i - 3));

            /* Compute (term1 + term3) - term2 safely */
            seq_t sum13 = term1 + term3;
            cache[i] = sum13 - term2;
        }
    }

    return cache[n];
}
