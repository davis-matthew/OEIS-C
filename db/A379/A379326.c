/*
 * A379326  G.f. A(x) satisfies A(x) = sqrt( (1 + 2*x) * (1 + 2*x*A(x)) ).
 * https://oeis.org/A379326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=2, for n>=2 a(n) = a(n-1) + 2*a(n-2) -
 *            (1/2)*sum_{k=1}^{n-1} a(k)*a(n-k)
 * Range:     i8 n <= 13, i16 n <= 22, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t  cache_len = 0;     /* number of valid terms in cache */
static idx_t  cache_cap = 0;     /* allocated capacity */

/* Ensure the cache holds terms up to index n (inclusive), computing missing ones. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_len) return;                   /* already computed */

    /* grow capacity if needed */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap <= n) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* initialise known terms if starting fresh */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
        if (n >= 1) {
            cache[1] = 2;
            cache_len = 2;
        }
    }

    /* compute terms from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;
        } else if (i == 1) {
            val = 2;
        } else {
            seq_t sum = 0;
            for (idx_t k = 1; k < i; ++k) {
                sum += cache[k] * cache[i - k];
            }
            val = cache[i - 1] + (seq_t)2 * cache[i - 2] - (sum >> 1);
        }
        cache[i] = val;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
