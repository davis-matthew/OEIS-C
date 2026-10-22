/*
 * A385837  a(n) = 1 + Sum_{k=0..n-1} (1 + k^4) * a(k) * a(n-1-k).
 * https://oeis.org/A385837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-1} (1 + k^4) * a(k) * a(n-1-k), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size of cache */
    static idx_t  used  = 0;   /* number of computed terms */

    /* allocate initial cache size if needed */
    if (cache == NULL) {
        cap   = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* grow cache to accommodate up to n */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        /* enlarge until it can hold all required elements */
        while (newcap <= (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure – stop computation */
            abort();
        }
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms */
    for (idx_t i = used; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else {
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k) {
                /* k^4 */
                seq_t kk = (seq_t)k * (seq_t)k;   /* k^2 */
                seq_t k4 = kk * kk;               /* k^4 */
                seq_t term = (1 + k4) * cache[k] * cache[i - 1 - k];
                sum += term;
            }
            cache[i] = 1 + sum;
        }
    }
    used = n + 1;
    return cache[n];
}
