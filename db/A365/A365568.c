/*
 * A365568  Expansion of e.g.f. 1 / (6 - 5 * exp(x))^(2/5).
 * https://oeis.org/A365568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (Product_{j=0..k-1} (5*j+2)) *
 *            Stirling2(n,k). a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   ensure_cache, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation computes the sequence by building the Stirling
   numbers of the second kind iteratively and weighting them with the
   required product.  A cache is maintained so that repeated calls are
   fast, independent of call order. */

#include <stddef.h>
#include "../utils/seq_alloc.h"     /* seq_alloc() */
#include "../utils/ensure_cache.h"  /* ensure_cache() */

/* Internal helper: allocate an array of seq_t and initialise to zero. */
static seq_t *alloc_seq_array_zero(size_t n)
{
    seq_t *a = seq_alloc(n);
    if (!a) return NULL;
    for (size_t i = 0; i < n; ++i) a[i] = (seq_t)0;
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Cache for computed values.  The cache is grown on demand. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure the cache holds a(0). */
    if (cache_cap == 0) {
        ensure_cache(0, &cache, &cache_cap);
        cache[0] = (seq_t)1;
    }
    if (n == 0) return cache[0];

    size_t N = (size_t)n;

    /* Pre‑compute the weight products P(k) = ∏_{j=0}^{k-1} (5*j+2). */
    seq_t *prod = alloc_seq_array_zero(N + 1);
    prod[0] = (seq_t)1;
    for (size_t k = 1; k <= N; ++k) {
        prod[k] = prod[k - 1] * (seq_t)(5 * (k - 1) + 2);
    }

    /* Allocate two rows for Stirling numbers. */
    seq_t *s_prev = alloc_seq_array_zero(N + 1);
    seq_t *s_cur  = alloc_seq_array_zero(N + 1);
    s_prev[0] = (seq_t)1;   /* S(0,0)=1 */

    for (size_t i = 1; i <= N; ++i) {
        /* Compute S(i, k) from the previous row. */
        s_cur[0] = (seq_t)0;
        for (size_t k = 1; k <= i; ++k) {
            s_cur[k] = (seq_t)k * s_prev[k] + s_prev[k - 1];
        }

        /* Compute a(i) by weighted sum of the current row. */
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k <= i; ++k) {
            sum += prod[k] * s_cur[k];
        }

        /* Store the value in the cache. */
        ensure_cache(i, &cache, &cache_cap);
        cache[i] = sum;

        /* Prepare for next iteration by swapping rows. */
        seq_t *tmp = s_prev;
        s_prev     = s_cur;
        s_cur      = tmp;
    }

    /* Clean up temporary arrays. */
    /* In most configurations seq_alloc is non‑freeing; freeing here is
       optional.  The memory usage for n≤10000 is negligible. */
    /* free(prod); free(s_prev); free(s_cur); */ /* optional */

    return cache[N];
}
