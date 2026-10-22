/*
 * A346051  G.f. A(x) satisfies: A(x) = 1 + x^2 + x^3 * A(x/(1 - x)) / (1 -
 *          x).
 * https://oeis.org/A346051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n-3} binomial(n-3,k) * a(k) (n ≥ 3) Initial
 *            values: a(0)=1, a(1)=0, a(2)=1
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 22, u64 n <= 35, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache of computed terms */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;   /* allocated size */
static idx_t  max_computed = 2; /* largest index that has a valid value */

/* Ensure the cache is allocated and the base values are stored. */
static void
init_cache(void)
{
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
    }
}

/* Allocate enough space for indices up to n inclusive. */
static void
ensure_cache_size(idx_t n)
{
    if (cache_size <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* out of memory: abort as the build environment expects valid */
            abort();
        }
        cache = tmp;
        cache_size = new_size;
    }
}

/* The sequence function. */
seq_t a(idx_t n)
{
    init_cache();
    ensure_cache_size(n);

    /* Return quickly for already computed indices. */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Compute new values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        idx_t m = i - 3;          /* terms in the sum */
        seq_t c = (seq_t)1;       /* C(m,0) */
        seq_t sum = cache[0];     /* c * cache[0] */

        for (idx_t k = 1; k <= m; ++k) {
            /* c = c * (m - k + 1) / k  (exact division) */
            c = (c * (seq_t)(m - k + 1)) / (seq_t)k;
            sum += c * cache[k];
        }
        cache[i] = sum;
    }

    max_computed = n;
    return cache[n];
}
