/*
 * A279358  Exponential transform of the cubes A000578.
 * https://oeis.org/A279358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=∑_{k=1..n} binom(n-1,k-1) * k^3 * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (binom(m,r) is the binomial coefficient C(m,r) ) */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for computed terms and bookkeeping */
static idx_t  cache_capacity = 0;
static idx_t  last_computed = 0;   /* largest n already computed (>=0) */
static seq_t *a_cache       = NULL;  /* a_cache[0..] : sequence values */

/* Grow the cache to at least new_cap entries */
static void ensure_capacity(idx_t new_cap)
{
    if (new_cap <= cache_capacity)
        return;

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return;            /* memory fault is ignored – build harness handles it */
    a_cache = tmp;

    /* initialise any new slots to 0 */
    for (idx_t i = cache_capacity; i < new_cap; ++i)
        a_cache[i] = (seq_t)0;

    /* base case, if this is the very first allocation */
    if (cache_capacity == 0)
        a_cache[0] = (seq_t)1;   /* a(0) = 1 */

    cache_capacity = new_cap;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure storage */
    ensure_capacity(n + 1);

    if (n <= last_computed)        /* already computed */
        return a_cache[n];

    /* Compute all terms from last_computed+1 up to n */
    for (idx_t cur = last_computed + 1; cur <= n; ++cur) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;      /* C(cur-1,0) = 1 for k=1 */

        for (idx_t k = 1; k <= cur; ++k) {
            seq_t k_val = (seq_t)k;
            seq_t k_cube = k_val * k_val * k_val;            /* k^3 */
            seq_t term = binom * k_cube * a_cache[cur - k];
            sum += term;

            if (k < cur) {
                /* binom = C(cur-1,k) = binom * (cur-k) / k */
                binom = (binom * (seq_t)(cur - k)) / (seq_t)k;
            }
        }
        a_cache[cur] = sum;
    }

    last_computed = n;
    return a_cache[n];
}
