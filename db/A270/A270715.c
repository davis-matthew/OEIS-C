/*
 * A270715  a(n) = ((n+2)/2)*Sum_{k=0..n/2}(Sum_{i=0..n-2*k}
 *          binomial(k+1,n-2*k-i)*binomial(k+i,k))/(k+1).
 * https://oeis.org/A270715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-1) - a(n-4); a(0)=1, a(1)=3, a(2)=5, a(3)=10
 * 
 * The sequence satisfies the linear recurrence a(n)=2*a(n-1)-a(n-4) (n≥4)
 * derived from its rational generating function.
 */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;       /* cached terms */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_len = 0;      /* number of computed terms */

/* compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* initialise cache with first four terms */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;           /* malloc failed – return 0 as fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)10;
        cache_len = 4;
    }

    /* if the requested index is already computed, return it */
    if ((size_t)n < cache_len)
        return cache[n];

    /* expand cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* realloc failed – return 0 as fallback */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* i ≥ 4 because cache_len is at least 4 */
        cache[i] = (seq_t)2 * cache[i-1] - cache[i-4];
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
