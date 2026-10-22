/*
 * A393977  G.f. A(x) satisfies A(x) = 1 - E^3(log(1 - x*A(x))), where E is
 *          the Euler operator x*d/dx.
 * https://oeis.org/A393977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = n^3 * a(n-1) + Σ_{k=1}^{n-1} (n/k)^2 * a(k) *
 *            a(n-1-k) (n>0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* dynamic array for computed values */
static size_t cache_len = 0;    /* number of elements stored */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* ensure the cache has at least n+1 elements */
    if (n + 1 > cache_len) {
        if (cache_len != 0)
            cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        else
            cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failed: return zero (caller must not expect this case) */
        /* initialise newly allocated part to zero */
        for (size_t i = cache_len; i <= n; ++i) cache[i] = 0;
        cache_len = n + 1;
    }

    /* base case */
    if (n == 0) {
        if (cache[0] == 0) cache[0] = 1;
        return cache[0];
    }

    /* if already computed, return cached value */
    if (cache[n] != 0) return cache[n];

    /* compute a(n) using the recurrence */
    seq_t term1 = ((seq_t)n * (seq_t)n * (seq_t)n) * cache[n - 1];
    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t prod = (seq_t)n * (seq_t)n * cache[k] * cache[n - 1 - k];
        prod = prod / ((seq_t)k * (seq_t)k);
        sum += prod;
    }
    cache[n] = term1 + sum;
    return cache[n];
}
