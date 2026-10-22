/*
 * A386229  G.f. A(x) satisfies A(x) = 1/( (1-x)^2 * (1 - x*A(x) - x^2*A'(x))
 *          ).
 * https://oeis.org/A386229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 1 + Σ_{k=0}^{n-1} (1 + k) * a(k) * a(n-1-k) (OEIS
 *            A386229)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Permanent cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity (in elements)   */
    static size_t size  = 0;   /* number of elements actually filled */

    /* Initial allocation: a(0) = 1 */
    if (cache == NULL) {
        cap   = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failed */
        cache[0] = (seq_t)1;
        size = 1;
    }

    /* Grow the cache if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap : 1;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = (idx_t)size; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t prod = cache[k] * cache[i - 1 - k];
            prod *= (seq_t)(k + 1);
            sum += prod;
        }
        cache[i] = (seq_t)(i + 1) + sum;
    }
    size = (size_t)n + 1;

    return cache[n];
}
