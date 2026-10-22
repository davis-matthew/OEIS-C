/*
 * A336217  a(0) = 1; a(n) = 2 * Sum_{k=0..n-1} binomial(n,k)^2 * a(k).
 * https://oeis.org/A336217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2 * Σ_{k=0}^{n-1} binomial(n,k)^2 * a(k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Use the utility where available, otherwise fall back to malloc/realloc.
   The compiler will provide seq_alloc_new_array, seq_realloc if the build
   environment supplies them, but we cannot depend on them being present.
*/

static seq_t *cache = NULL;          /* array of cached a(n) values      */
static size_t cache_capacity = 0;    /* current allocated capacity       */
static idx_t  computed_max = 0;      /* largest n for which cache[n] is valid */

/* Ensure that the cache can hold values up to n.  The first allocation
   reserves (n+1) slots; subsequent growth doubles the capacity. */
static void ensure_cache_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;

    if (cache == NULL) {
        cache_capacity = need;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (cache == NULL) abort();                 /* out of memory */
        for (size_t i = 0; i < cache_capacity; ++i)
            cache[i] = 0;
        cache[0] = (seq_t)1;  /* a(0) = 1 */
        computed_max = 0;
    } else if (need > cache_capacity) {
        size_t new_cap = cache_capacity * 2;
        if (need > new_cap) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        for (size_t i = cache_capacity; i < new_cap; ++i)
            cache[i] = 0;
        cache_capacity = new_cap;
    }
}

/* Compute the n‑th term of A336217, memoizing all intermediate values. */
seq_t a(idx_t n)
{
    ensure_cache_capacity(n);

    if (n <= computed_max)
        return cache[n];

    /* Compute values from (computed_max + 1) up to n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t binom = (seq_t)1;  /* binomial(i,0) */

        for (idx_t k = 0; k < i; ++k) {
            /* term = binomial(i,k)^2 * a(k) */
            seq_t term = binom * binom;
            term *= cache[k];
            sum += term;

            /* update binomial: binom = binomial(i, k+1) */
            if (k + 1 < i) {
                binom = (binom * (seq_t)(i - k)) / (seq_t)(k + 1);
            }
        }

        cache[i] = sum * (seq_t)2;   /* a(i) = 2 * sum */
    }

    computed_max = n;
    return cache[n];
}
