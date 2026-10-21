/*
 * A291287  a(0)=a(1)=1, a(2)=3, thereafter a(n) =
 *          n*a(n-1)+(n-1)*(n-2)*a(n-2).
 * https://oeis.org/A291287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(n)=n*a(n-1)+(n-1)*(n-2)*a(n-2) for
 *            n>=3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoization table */
static idx_t  max_computed = 2;      /* highest index that has been computed */
static size_t cap = 0;               /* current capacity of cache (elements) */

seq_t a(idx_t n)
{
    /* initialise cache if needed */
    if (cache == NULL) {
        /* make room for indices 0..max(2,n) */
        cap = n + 1;
        if (cap < 3) cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;         /* should not happen */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 3;
        max_computed = 2;
    }

    /* grow cache if necessary */
    if (n >= cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* should not happen */
        cache = tmp;
        cap = new_cap;
    }

    /* compute any missing values up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t term1 = (seq_t)i * cache[i - 1];
            seq_t term2 = ((seq_t)(i - 1)) * ((seq_t)(i - 2)) * cache[i - 2];
            cache[i] = term1 + term2;
        }
        max_computed = n;
    }

    return cache[n];
}
