/*
 * A393527  a(0) = a(1) = a(2) = 1; thereafter odd indexed terms are the sum
 *          of the last 3 terms and even indexed terms are the sum of all
 *          previous terms.
 * https://oeis.org/A393527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=a(2)=1; a(n)=a(n-1)+a(n-2)+a(n-3) for odd n>=3;
 *            a(n)=sum_{k=0}^{n-1} a(k) for even n>=4;
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* memoisation table */
static idx_t cache_used = 0;         /* highest index computed so far */
static size_t cache_cap  = 0;        /* allocated capacity of cache */
static seq_t  cum_sum     = (seq_t)0;/* sum of all a[0..cache_used] */

seq_t a(idx_t n)
{
    /* initialise memoisation structures if needed */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();

        /* base values a(0), a(1), a(2) */
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cum_sum = cache[0] + cache[1] + cache[2];
        cache_used = 2;
    }

    /* If the requested n is already known, return it directly */
    if (n <= cache_used)
        return cache[n];

    /* Grow the cache array if required */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n, using memoised smaller terms */
    for (idx_t i = cache_used + 1; i <= n; ++i) {
        if (i & 1) {                    /* odd index: sum of last three terms */
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
        } else {                        /* even index: sum of all previous terms */
            cache[i] = cum_sum;
        }
        cum_sum += cache[i];            /* update sum for next even indices */
    }

    cache_used = n;
    return cache[n];
}
