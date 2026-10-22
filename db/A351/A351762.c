/*
 * A351762  Expansion of e.g.f. 1/(1 - 2*x*exp(x)).
 * https://oeis.org/A351762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 2*n*Sum_{k=0}^{n-1} binomial(n-1,k)*a(k) for
 *            n>0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS sequence A351762 (expansion of 1/(1-2*x*exp(x)). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size (number of elements)   */
    static idx_t computed = 0;     /* highest n that has been computed so far */

    /* Ensure cache is allocated with at least n+1 entries */
    if (n + 1 > cache_size) {
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;   /* out of memory – terminate with zero (unlikely) */
    }

    /* Initialise a(0) if not already done */
    if (computed == 0 && cache[0] == (seq_t)0) {
        cache[0] = (seq_t)1;
        computed = 0;
    }

    /* Compute any missing terms up to n */
    for (idx_t m = computed + 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t binom = 1;                 /* binomial(m-1,0) */
        for (idx_t k = 0; k < m; ++k) {  /* k runs 0 .. m-1 */
            sum += binom * cache[k];
            if (k + 1 < m) {
                /* binom(m-1,k+1) = binom(m-1,k) * (m-1-k) / (k+1) */
                binom = (binom * (m - 1 - k)) / (k + 1);
            }
        }
        cache[m] = 2 * (seq_t)m * sum;
    }

    computed = n;
    return cache[n];
}
