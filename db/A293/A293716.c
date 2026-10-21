/*
 * A293716  E.g.f.: exp(x + 2*x^2 + 3*x^3).
 * https://oeis.org/A293716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n) = a(n-1) + 4*(n-1)*a(n-2) +
 *            9*(n-1)*(n-2)*a(n-3) (terms with negative indices are omitted)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache for memoisation */
static seq_t *cache = NULL;          /* array of computed a(n) */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t cache_size = 0;         /* number of values already computed */

/* Ensure that a(n) is available in the cache. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;          /* already computed */

    /* grow cache if necessary */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return;      /* allocation failed – caller gets 0 */
    } else if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return;      /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill missing terms */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }
    if (n >= 1 && cache_size <= 1) {
        cache[1] = (seq_t)1;
        cache_size = 2;
    }
    if (n >= 2 && cache_size <= 2) {
        cache[2] = (seq_t)5;
        cache_size = 3;
    }

    /* compute terms from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        /* term a(i-1) */
        sum += cache[i-1];

        /* term 4*(i-1)*a(i-2) if i>=2 */
        if (i >= 2) {
            seq_t factor = 4 * (seq_t)(i - 1);
            sum += cache[i-2] * factor;
        }

        /* term 9*(i-1)*(i-2)*a(i-3) if i>=3 */
        if (i >= 3) {
            seq_t factor = 9 * (seq_t)(i - 1) * (seq_t)(i - 2);
            sum += cache[i-3] * factor;
        }

        cache[i] = sum;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache == NULL)
        return (seq_t)0;      /* allocation failure */
    return cache[n];
}
