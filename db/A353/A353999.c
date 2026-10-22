/*
 * A353999  Expansion of e.g.f. 1/(1 - x^3/6 * (exp(x) - 1)).
 * https://oeis.org/A353999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,3) * Sum_{k=4..n} binomial(n-3,k-3) * a(n-k)
 *            with a(0)=1, a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of previously computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* allocated size of cache   */
static idx_t  cache_max = (idx_t)-1;  /* highest index computed   */

/* Ensure the cache can hold indices up to n. Allocate or grow as necessary. */
static void ensure_cache_capacity(idx_t n)
{
    /* If the cache has never been allocated, create an initial block. */
    if (cache == NULL) {
        cache_cap = (n < 4) ? 4 : (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();  /* terminate on allocation failure */

        /* initialise the first four terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache_max = 3;
        return;
    }

    /* If the current capacity is insufficient, grow it. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();  /* terminate on allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Compute a(n) for all indices up to n that have not yet been computed. */
static void compute_up_to(idx_t n)
{
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* binom(i,3) = i*(i-1)*(i-2)/6 */
        seq_t binom_n3 = ((seq_t)i * (seq_t)(i - 1) * (seq_t)(i - 2)) / (seq_t)6;

        seq_t sum = 0;
        seq_t binom_val = 1;                 /* binom(i-3, 0) */
        idx_t max_j = i - 4;                 /* j ranges 0 .. i-4 */

        for (idx_t j = 0; j <= max_j; ++j) {
            /* add term a[j] * binom(i-3, j) */
            if (cache[j] != (seq_t)0)
                sum += cache[j] * binom_val;

            /* update binom_val to binom(i-3, j+1) for next iteration */
            if (j < max_j) {
                binom_val = binom_val * (seq_t)(i - 3 - j);
                binom_val = binom_val / (seq_t)(j + 1);
            }
        }

        cache[i] = binom_n3 * sum;
    }
    cache_max = n;
}

/* External linkage function required by the build. */
seq_t a(idx_t n)
{
    if (n <= 3) {
        /* The first four terms are known a priori. */
        if (cache == NULL)
            ensure_cache_capacity(3);
        return cache[n];
    }

    ensure_cache_capacity(n);
    if (cache_max < n)
        compute_up_to(n);

    return cache[n];
}
