/*
 * A386451  G.f. A(x) satisfies A(x) = 1/(1 - x - x^6*A'''''(x)).
 * https://oeis.org/A386451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A386451
 * Recurrence:
 *   a(0) = 1
 *   a(n) = a(n-1) + Σ_{k=0}^{n-1} (24*k - 50*k^2 + 35*k^3 - 10*k^4 + k^5) * a(k) * a(n-1-k)
 * Offset: 0
 *
 * This implementation memoizes previously computed terms in a static
 * array, allowing calls with indices in arbitrary order.  The algorithm
 * is O(n^2) in the worst case and works for n up to about 10 000 within
 * the time limit.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed = 0;   /* highest index computed so far */

/* Ensure cache has space for indices 0..n and initialize a[0] if needed. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Initial allocation: at least 1 element for a[0] */
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        computed = 0;
    }
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main function requested by the build system */
seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;   /* safeguard, though idx_t is unsigned */

    ensure_cache(n);

    if (n == 0) return cache[0];

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t kmax = i - 1;
        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t kk = (seq_t)k;
            /* Polynomial coefficient: 24*k - 50*k^2 + 35*k^3 - 10*k^4 + k^5 */
            seq_t coeff = 24 * kk
                        - 50 * kk * kk
                        + 35 * kk * kk * kk
                        - 10 * kk * kk * kk * kk
                        + kk * kk * kk * kk * kk;
            seq_t prod = coeff * cache[k] * cache[kmax - k];
            sum += prod;
        }
        cache[i] = cache[i - 1] + sum;
    }
    computed = n;
    return cache[n];
}
