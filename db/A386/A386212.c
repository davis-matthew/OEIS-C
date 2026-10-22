/*
 * A386212  G.f. A(x) satisfies A(x) = 1/(1-x)^3 + x^2*A(x)*A'(x).
 * https://oeis.org/A386212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+2,2) + ∑_{k=0}^{n-1} k * a(k) * a(n-1-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 3, a(2) = 9, a(3) = 37, ...  (OEIS A386212) */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation table */
    static size_t cap   = 0;      /* current capacity */

    if (n >= cap) {
        size_t oldcap = cap;
        size_t newcap = n + 1;                 /* need indices 0..n */

        seq_t *newcache = (seq_t*)malloc(newcap * sizeof(seq_t));
        if (!newcache) abort();                /* allocation failure */

        if (cache) {
            memcpy(newcache, cache, oldcap * sizeof(seq_t));
            free(cache);
        }
        cache = newcache;
        cap   = newcap;

        /* Compute all missing terms from oldcap to n */
        for (idx_t i = oldcap; i <= n; ++i) {
            seq_t sum = 0;

            for (idx_t k = 0; k < i; ++k) {
                seq_t prod = cache[k] * cache[i - 1 - k];
                prod *= (seq_t)k;                 /* weight k */
                sum += prod;
            }

            seq_t bin = ((seq_t)(i + 2) * (seq_t)(i + 1)) / 2;
            cache[i] = bin + sum;
        }
    }
    return cache[n];
}
