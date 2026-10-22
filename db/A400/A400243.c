/*
 * A400243  G.f. A(x) satisfies A(x) = A(x^2/(1 - x)^3) / (1 - x)^2, with A(0)
 *          = 1.
 * https://oeis.org/A400243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n + k + 1, 3*k + 1) * a(k)
 *            with a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Uses a simple memoised recurrence; no other files are required.   */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;  /* memoised a(k) values, 0..cache_len-1    */
static idx_t cache_len = 0;  /* number of currently stored values      */
static size_t cache_cap = 0; /* capacity of the cache array (in elements) */

/* Ensure entries a(0) … a(n) are present in cache.  */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    /* Allocate/expand the cache if necessary. */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache = tmp;
        cache_cap = needed;
    }

    /* Initialise a(0) if the array was empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t maxk = i >> 1;           /* floor(i/2) */

        for (idx_t k = 0; k <= maxk; ++k) {
            sum += binomial(i + k + 1, 3 * k + 1) * cache[k];
        }
        cache[i] = sum;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
