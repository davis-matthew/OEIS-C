/*
 * A352471  a(0) = 1; a(n) = Sum_{k=0..floor((n-1)/2)} binomial(n,2*k+1)^3 *
 *          a(n-2*k-1).
 * https://oeis.org/A352471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=0..⌊(n-1)/2⌋} binomial(n,2k+1)^3 *
 *            a(n-2k-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;          /* memoization array */
static size_t cache_capacity = 0;    /* current allocated size of cache */
static idx_t computed_upto = (idx_t)-1; /* highest index computed so far */

/* Ensure the cache can store indices up to (needed-1). */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_capacity) return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 1;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);          /* allocation failed */
    cache = tmp;
    cache_capacity = new_cap;
}

/* Sequence a(n) as defined above. */
seq_t a(idx_t n)
{
    /* Special handle for n = 0: a(0) = 1. */
    if (n == 0) return (seq_t)1;

    /* First call: set up cache and initial value. */
    if (cache == NULL) {
        ensure_cache((size_t)n + 1);
        cache[0] = (seq_t)1;
        computed_upto = 0;
    }

    /* Compute any missing values up to n. */
    if (n > computed_upto) {
        /* Expand cache if required. */
        if ((size_t)n + 1 > cache_capacity)
            ensure_cache((size_t)n + 1);

        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            idx_t max_k = (i - 1) / 2;
            for (idx_t k = 0; k <= max_k; ++k) {
                idx_t j = 2 * k + 1;           /* odd index */
                seq_t comb = binomial(i, j);    /* C(i, j) */
                seq_t term = comb * comb * comb;/* comb^3 */
                term = term * cache[i - j];     /* * a(i-j) */
                sum = sum + term;
            }
            cache[i] = sum;
        }
        computed_upto = n;
    }

    return cache[n];
}
