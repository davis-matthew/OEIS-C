/*
 * A322189  G.f. A(x) satisfies: A(x)^2 + A(x) - 1 = Sum_{n>=0}
 *          binomial(3*n,n)^2 * x^n.
 * https://oeis.org/A322189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n>=1, a(n) = (binomial(3*n, n)^2 - Σ_{i=1}^{n-1}
 *            a(i)*a(n-i)) / 3 where the sum is over positive i
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t len = 0;   /* number of values stored (cache[0..len-1]) */

    if (len == 0) {
        capacity = 4;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = 1;
        len = 1;
    }

    if (n < len)
        return cache[n];

    /* extend cache up to n */
    if (capacity <= (size_t)n) {
        capacity = n * 2 + 4;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    }

    for (idx_t k = len; k <= n; ++k) {
        /* compute g_k = binomial(3*k, k)^2 */
        seq_t t = binomial(3 * k, k);
        seq_t g = t * t;

        /* compute sum_{i=1}^{k-1} a[i] * a[k-i] */
        seq_t sum = 0;
        for (idx_t i = 1; i < k; ++i) {
            sum += cache[i] * cache[k - i];
        }

        /* a_k = (g - sum) / 3 */
        cache[k] = (g - sum) / 3;
    }

    len = n + 1;
    return cache[n];
}
