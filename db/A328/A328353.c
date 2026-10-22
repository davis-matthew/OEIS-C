/*
 * A328353  a(n)*S is the sum of all positive integers whose decimal expansion
 *          is up to n digits and uses six distinct nonzero digits
 *          d1,d2,d3,d4,d5,d6 such that d1+d2+d3+d4+d5+d6=S.
 * https://oeis.org/A328353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 61*a(n-1) - 60*a(n-2) + 6^(n-1) for n > 1, a(0) = 0, a(1)
 *            = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation buffer */
    static seq_t *cache = NULL;         /* array of computed terms */
    static idx_t  cache_len = 0;        /* number of terms that are valid */
    static idx_t  cache_cap = 0;        /* allocated capacity */
    static seq_t  pow6 = 0;             /* 6^(cache_len-1) when cache_len>=2 */

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 2;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]  = (seq_t)0;
        cache[1]  = (seq_t)1;
        cache_len = 2;          /* we have a(0) and a(1) */
        pow6      = (seq_t)6;   /* 6^(2-1) */
    }

    /* grow cache and compute missing terms up to n */
    while (cache_len <= n) {
        /* enlarge capacity if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2 + 2;
            seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
            for (idx_t i = 0; i < cache_len; ++i)
                tmp[i] = cache[i];
            free(cache);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute a(cache_len) using the recurrence */
        idx_t cur = cache_len;      /* next index to compute */
        seq_t term = (seq_t)61 * cache[cur-1] - (seq_t)60 * cache[cur-2] + pow6;
        cache[cur] = term;

        /* update pow6 for the next term: pow6 = 6^(cur) */
        pow6 = pow6 * (seq_t)6;

        ++cache_len;
    }

    return cache[n];
}
