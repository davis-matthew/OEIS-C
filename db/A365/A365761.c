/*
 * A365761  G.f. satisfies A(x) = 1 + x*A(x)*(1 + x^2*A(x)^5).
 * https://oeis.org/A365761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n-2*k,k) *
 *            binomial(n+3*k+1,n-2*k) / (n+3*k+1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of elements already computed */
    static size_t cache_cap = 0; /* allocated capacity */

    /* first call: allocate cache[0] = 1 */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = 1;
        cache_len = 1;
    }

    /* grow cache if necessary */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        idx_t maxk = i / 3;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t t1 = binom(i - 2 * k, k);
            seq_t t2 = binom(i + 3 * k + 1, i - 2 * k);
            seq_t prod = t1 * t2;
            seq_t denom = (seq_t)(i + 3 * k + 1);
            seq_t term = prod / denom;
            sum += term;
        }
        cache[i] = sum;
    }
    cache_len = n + 1;

    return cache[n];
}
