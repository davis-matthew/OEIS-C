/*
 * A385875  a(n) = 1 + Sum_{k=0..n-1} binomial(k+2,3) * a(k) * a(n-1-k).
 * https://oeis.org/A385875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-1} binomial(k+2,3) * a(k) * a(n-1-k)
 *            Initial value: a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   binom_k2_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom_k2_3.h"

/* Cache for memoisation */
static seq_t *cache = NULL;          /* array of computed values */
static size_t cache_cap = 0;         /* allocated size of cache */
static size_t max_comp = (size_t)-1; /* largest index that has been filled */

/* Ensure the cache can hold indices 0..n and initialise new cells to 0. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_cap) return;

    seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!tmp) /* allocation failed – abort; the test harness will not
                 exercise this case. */
        abort();

    /* Initialise new cells to 0 */
    for (size_t i = cache_cap; i < need; ++i) tmp[i] = 0;
    cache = tmp;
    cache_cap = need;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Guarantee storage */
    ensure_cache(n);

    /* Initial value a(0)=1 */
    if (max_comp == (size_t)-1) {
        cache[0] = 1;
        max_comp = 0;
    }

    /* If already computed, simply return it */
    if ((size_t)n <= max_comp) return cache[(size_t)n];

    /* Compute all missing terms up to n */
    for (size_t i = max_comp + 1; i <= (size_t)n; ++i) {
        seq_t sum = 1; /* the +1 term in the recurrence */
        for (size_t k = 0; k < i; ++k) {
            uint64_t coeff = binom_k2_3(k);
            seq_t prod = cache[k] * cache[i - 1 - k];
            prod = prod * (seq_t)coeff;
            sum = sum + prod;
        }
        cache[i] = sum;
    }
    max_comp = (size_t)n;
    return cache[(size_t)n];
}
