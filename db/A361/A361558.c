/*
 * A361558  Expansion of e.g.f. exp((x + x^2/2 + x^3/6 + x^4/24)/(1-x)).
 * https://oeis.org/A361558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1) * a(n-1) - (n-1)*(n-3) * a(n-2) - binomial(n-1,3)
 *            * a(n-4) - 3*binomial(n-1,4) * a(n-5), for n > 4 Initial values:
 *            a(0)=1, a(1)=1, a(2)=4, a(3)=20, a(4)=127
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_cap = 0;    /* allocated capacity */
static size_t cache_len = 0;    /* number of terms computed (>=0) */

static void grow_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) /* allocation failed */
        return; /* silently. The calling code will get incomplete values. */
    cache = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Lazy initialization of first five terms */
    if (cache_len == 0) {
        grow_cache(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)20;
        cache[4] = (seq_t)127;
        cache_len = 5;
    }

    /* Expand the cache up to n if needed */
    if ((idx_t)cache_len <= n)
        grow_cache(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)4;
        } else if (i == 3) {
            cache[i] = (seq_t)20;
        } else if (i == 4) {
            cache[i] = (seq_t)127;
        } else {
            seq_t term1 = (seq_t)(2 * i - 1) * cache[i-1];
            seq_t term2 = (seq_t)(i-1) * (seq_t)(i-3) * cache[i-2];
            seq_t term3 = binomial(i-1, 3) * cache[i-4];
            seq_t term4 = (seq_t)3 * binomial(i-1, 4) * cache[i-5];
            cache[i] = term1 - term2 - term3 - term4;
        }
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
