/*
 * A342110  a(n) = Sum_{k=0..n} Stirling2(n,k) * Stirling2(n,n-k).
 * https://oeis.org/A342110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n,k) * Stirling2(n,n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A342110 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static local state */
    static seq_t *st_prev = NULL;      /* previous row of Stirling numbers */
    static seq_t *st_curr = NULL;      /* current row being computed */
    static size_t st_cap = 0;          /* allocated capacity for stirling arrays */

    static seq_t *cache = NULL;        /* cache of a(n) values */
    static size_t cache_cap = 0;       /* allocated capacity for cache */

    static idx_t max_done = 0;         /* highest n already computed */

    /* helper to ensure capacity of stirling arrays */
    if (st_cap < n + 1) {
        size_t needed = n + 1;
        seq_t *new_prev, *new_curr;
#ifdef __cplusplus
        new_prev = new seq_t[needed]();
        new_curr = new seq_t[needed]();
        if (st_prev) {
            for (size_t i = 0; i < st_cap; ++i) new_prev[i] = st_prev[i];
            for (size_t i = 0; i < st_cap; ++i) new_curr[i] = st_curr[i];
            delete [] st_prev;
            delete [] st_curr;
        }
#else
        new_prev = (seq_t*)realloc(st_prev, needed * sizeof(seq_t));
        new_curr = (seq_t*)realloc(st_curr, needed * sizeof(seq_t));
#endif
        st_prev = new_prev;
        st_curr = new_curr;
        st_cap  = needed;
    }

    /* helper to ensure capacity of cache */
    if (cache_cap < n + 1) {
        size_t needed = n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[needed]();
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
            delete [] cache;
        }
#else
        new_cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
#endif
        cache = new_cache;
        cache_cap = needed;
    }

    /* initialise state for n = 0 if not yet done */
    if (max_done == 0) {
        /* S(0,0) = 1 */
        st_prev[0] = 1;
        cache[0]   = 1;
        max_done = 0;
    }

    /* compute rows up to n */
    for (idx_t i = max_done + 1; i <= n; ++i) {
        /* compute Stirling numbers of the second kind for row i */
        st_curr[0] = 0;                     /* S(i,0) = 0 for i > 0 */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term1 = 0;
            if (k <= i - 1)                   /* S(i-1,k) exists */
                term1 = (seq_t)k * st_prev[k];
            seq_t term2 = st_prev[k - 1];      /* S(i-1,k-1) exists */
            st_curr[k] = term1 + term2;
        }

        /* compute a(i) = sum_{k=1}^{i-1} S(i,k) * S(i,i-k) */
        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k) {
            sum += st_curr[k] * st_curr[i - k];
        }
        cache[i] = sum;

        /* prepare for next iteration: swap prev and curr */
        seq_t *tmp = st_prev;
        st_prev   = st_curr;
        st_curr   = tmp;
    }

    max_done = n;
    return cache[n];
}
