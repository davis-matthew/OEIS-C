/*
 * A400441  G.f. A(x) satisfies A(x) = (1 + x*A(x^5))^2.
 * https://oeis.org/A400441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence form a(0) = 1; a(5*n+1) = 2 * a(n); a(5*n+2) =
 *            sum_{k=0..n} a(k) * a(n-k); a(5*n+3) = a(5*n+4) = a(5*n+5) = 0
 *            for n >= 0 (All other indices are handled by the same rule.)
 * Range:     u8 n <= 216, u16 n <= 1271, u32 n <= 7971, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated size in elements */
    static idx_t computed_upto = 0;   /* highest index computed so far */

    /* Initial allocation */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* a(0) = 1 */
        cache[0] = (seq_t)1;
    }

    /* Expand cache if necessary */
    if ((size_t)cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute values up to n (ascending order guarantees dependencies satisfied) */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        if (i % 5 == 1) {
            /* a(5*k+1) = 2 * a(k)  */
            cache[i] = cache[i / 5] * (seq_t)2;
        } else if (i % 5 == 2) {
            /* a(5*k+2) = Σ_{k=0..m} a(k)*a(m-k), where m = i/5 */
            idx_t m = i / 5;
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= m; ++k) {
                sum += cache[k] * cache[m - k];
            }
            cache[i] = sum;
        } else {
            /* a(5*k), a(5*k+3), a(5*k+4) = 0 for k >= 0 */
            cache[i] = (seq_t)0;
        }
    }
    computed_upto = n;

    return cache[n];
}
