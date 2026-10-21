/*
 * A294221  Exponential transform of the square pyramidal numbers (A000330).
 * https://oeis.org/A294221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponential transform of the square pyramidal numbers
 *            (A000330). It satisfies the recurrence a(0) = 1 a(n) =
 *            ∑_{k=1}^{n} C(n‑1,k‑1) * s_k * a(n‑k), where s_k =
 *            k(k+1)(2k+1)/6
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* static cache --------------------------------------------------- */
static seq_t *cache      = NULL;   /* array of computed a(n) values   */
static idx_t cache_size  = 0;      /* current size of the cache       */
static idx_t cache_max   = 0;      /* highest index already computed */

/* ensure the cache can hold indices up to n (inclusive)          */
static void ensure_capacity(idx_t n)
{
    if ( (size_t)n < cache_size )
        return;                     /* already large enough              */

    idx_t new_size = n + 1;
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_size];
#else
    new_cache = (seq_t*)malloc( (size_t)new_size * sizeof(seq_t) );
#endif

    /* copy previously computed values, if any */
    if (cache) {
        for (idx_t i = 0 ; i < cache_size ; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete [] cache;
#else
        free(cache);
#endif
    }

    cache     = new_cache;
    cache_size= new_size;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) is always 1                       */
        return 1;

    ensure_capacity(n);
    cache[0] = 1;              /* base value for the recurrence          */

    /* compute all missing values up to n */
    for (idx_t m = cache_max + 1 ; m <= n ; ++m) {
        seq_t sum   = 0;
        seq_t binom = 1;           /* C(m-1,0) */

        for (idx_t k = 1 ; k <= m ; ++k) {
            seq_t kk = (seq_t)k;
            seq_t s  = kk * (kk + 1) * ((seq_t)2 * kk + 1) / ((seq_t)6);
            seq_t term = s * binom * cache[m - k];
            sum += term;

            if (k < m) { /* update binomial: C(m-1,k) = C(m-1,k-1) * (m-k)/k */
                binom = binom * (seq_t)(m - k) / (seq_t)k;
            }
        }
        cache[m] = sum;
    }

    cache_max = n;
    return cache[n];
}
