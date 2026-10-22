/*
 * A357956  a(n) = 5*A005259(n) - 2*A005258(n).
 * https://oeis.org/A357956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*A005259(n) - 2*A005258(n) * A005259(n) = Σ_{k=0..n}
 *            binomial(n,k)^2 * binomial(n+k,k)^2 * A005258(n) = Σ_{k=0..n}
 *            binomial(n,k)^2 * binomial(n+k,k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for computed values */
static seq_t *a_cache = NULL;  /* array of a(0..cap-1) */
static size_t cache_cap = 0;   /* current allocated size */

/* Ensure the cache can hold up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_cap > n) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while ((idx_t)new_cap <= n) new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);           /* allocation failed */

    /* initialise new slots to zero (uncomputed) */
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;

    a_cache = tmp;
    cache_cap = new_cap;
}

/* Main function computing a(n) */
seq_t a(idx_t n)
{
    /* preallocate cache up to the requested index */
    ensure_cache(n);

    /* compute all necessary values */
    for (idx_t i = 0; i <= n; ++i) {
        if (i < (idx_t)cache_cap && a_cache[i] != 0)
            continue;   /* already computed */

        /* compute sums for this i */
        seq_t sum1 = 0;   /* Σ C^2 * B^2 */
        seq_t sum2 = 0;   /* Σ C^2 * B   */
        seq_t C = 1;      /* binomial(i,0)   */
        seq_t B = 1;      /* binomial(i+0,0) */

        for (idx_t k = 0; k <= i; ++k) {
            seq_t C2   = C * C;
            seq_t B2   = B * B;

            sum1 += C2 * B2;   /* C^2 * B^2 */
            sum2 += C2 * B;    /* C^2 * B  */

            if (k < i) {
                /* update for next k */
                seq_t n_minus_k = (seq_t)i - k;
                seq_t k1 = (seq_t)(k + 1);
                C = C * n_minus_k / k1;                   /* binomial(i,k+1)  */
                B = B * ((seq_t)i + k + 1) / k1;           /* binomial(i+k+1,k+1) */
            }
        }

        a_cache[i] = 5 * sum1 - 2 * sum2;
    }

    return a_cache[n];
}
