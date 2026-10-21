/*
 * A255927  a(n) = (3/4) * Sum_{k>=0} (3*k)^n/4^k.
 * https://oeis.org/A255927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 4 * Σ_{k=0}^{n-1} C(n-1,k) * a(k) * a(n-1-k) - 3
 *            * a(n-1) where C(n,k) is the binomial coefficient. This follows
 *            from the differential equation A'(x) = A(x)(4A(x)-3) for the
 *            exponential generating function A(x) = 3/(4-exp(3x))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed values */
static seq_t *cache = NULL;   /* array of a(n) */
static idx_t  cache_len = 0; /* number of elements stored (highest index + 1) */
static idx_t  cache_cap = 0; /* current allocation size */

/* Helper to ensure cache capacity */
static void ensure_capacity(idx_t needed)
{
    if (needed < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort(); /* allocation failure */
    /* Initialize new elements to zero */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_ptr[i] = (seq_t)0;
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main function */
seq_t a(idx_t n)
{
    if (n >= cache_len) {
        /* ensure we can store up to n */
        ensure_capacity(n + 1);

        /* compute values from cache_len to n inclusive */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            idx_t m = i - 1; /* number of terms in the sum */
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1; /* C(m,0) */

            for (idx_t k = 0; k <= m; ++k) {
                /* term = binom * a(k) * a(m-k) */
                seq_t term = binom * cache[k];
                term *= cache[m - k];
                sum += term;

                if (k < m) {
                    /* Update binom to C(m, k+1) */
                    /* binom = binom * (m - k) / (k + 1) */
                    seq_t num = binom * (seq_t)(m - k);
                    seq_t den = (seq_t)(k + 1);
                    binom = num / den; /* exact division */
                }
            }

            cache[i] = (seq_t)4 * sum - (seq_t)3 * cache[m];
        }

        cache_len = n + 1;
    }

    return cache[n];
}
