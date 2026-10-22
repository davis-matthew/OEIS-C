/*
 * A345456  a(n) = Sum_{k=0..n} binomial(5*n+2,5*k).
 * https://oeis.org/A345456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(5*n+2,5*k) Recurrence: a(0)=1,
 *            a(1)=22, a(2)=859, a(n)=21*a(n-1)+353*a(n-2)-32*a(n-3) for n>2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;   /* number of entries in cache */

seq_t a(idx_t n)
{
    /* initialise cache with first three terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)859;
    }

    /* return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* resize cache to accommodate n+1 entries */
    cache = (seq_t*)realloc(cache, sizeof(seq_t) * (size_t)(n + 1));

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)
            val = (seq_t)1;
        else if (i == 1)
            val = (seq_t)22;
        else if (i == 2)
            val = (seq_t)859;
        else
            val = 21*cache[i-1] + 353*cache[i-2] - 32*cache[i-3];
        cache[i] = val;
    }
    cache_len = (idx_t)(n + 1);

    return cache[n];
}
