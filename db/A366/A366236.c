/*
 * A366236  G.f. A(x) satisfies A(x) = 1 + x + x*(1 + x)^2*A(x)^2.
 * https://oeis.org/A366236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*k+1,n-k) * binomial(2*k,k)/(k+1)
 *            Sequence defined by the quadratic equation A(x) = 1 + x +
 *            x*(1+x)^2*A(x)^2. Uses incremental computation: a_0 = 1, a_1 = 2
 *            and a_n = d_{n-1} + 2*d_{n-2} + d_{n-3} (n ≥ 2) where d_k =
 *            Σ_{i=0..k} a_i*a_{k-i}
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache for already computed a(n) and d(n) values */
static seq_t *a_cache = NULL;
static seq_t *d_cache = NULL;
static idx_t cache_size = 0;   /* number of computed indices (last index + 1) */

/* Ensure the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* initialise cache for n = 0 */
    if (cache_size == 0) {
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        d_cache = (seq_t*)malloc(sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        d_cache[0] = (seq_t)1;
        cache_size = 1;
    }

    if (n + 1 <= cache_size)   /* already computed */
        return;

    /* grow the arrays to hold up to index n */
    a_cache = (seq_t*)realloc(a_cache, (size_t)(n + 1) * sizeof(seq_t));
    d_cache = (seq_t*)realloc(d_cache, (size_t)(n + 1) * sizeof(seq_t));

    /* compute values incrementally from cache_size up to n */
    for (idx_t k = cache_size; k <= n; ++k) {
        if (k == 1) {
            a_cache[1] = (seq_t)2;
            seq_t prod = a_cache[0] * a_cache[1];
            d_cache[1] = prod + prod;          /* 2 · a0·a1  */
        } else {
            /* compute a_k from previously stored d values */
            seq_t a_k = d_cache[k - 1];
            a_k += d_cache[k - 2] * 2;           /* 2·d_{k-2}   */
            if (k >= 3)
                a_k += d_cache[k - 3];          /* + d_{k-3}   */
            a_cache[k] = a_k;

            /* compute d_k = Σ_{i=0..k} a_i·a_{k-i} (use symmetry) */
            seq_t d_k = 0;
            size_t half = (size_t)k / 2;
            for (size_t i = 0; i <= half; ++i) {
                seq_t prod = a_cache[i] * a_cache[k - i];
                if (i == k - i)       /* middle term appears once */
                    d_k += prod;
                else                  /* symmetrical pair appears twice */
                    d_k += prod + prod;
            }
            d_cache[k] = d_k;
        }
    }

    cache_size = n + 1;
}

/* External function to return the n-th term (a(0) = 1). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
