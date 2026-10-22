/*
 * A348857  G.f. A(x) satisfies: A(x) = 1 / ((1 - x) * (1 - x * A(2*x))).
 * https://oeis.org/A348857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A348857
 * Recurrence:  a(0) = 1
 *              a(n) = 1 + Σ_{k=0}^{n-1} 2^k * a(k) * a(n-k-1)   for n ≥ 1
 */

#include <stdlib.h>
#ifndef __cplusplus
#include <stdint.h>
#endif

/* Compute a(n) for all n ≥ 0.  The function is thread‑unsafe but
 * memoises all previously computed values so that calls can be made
 * in any order and each value is computed only once. */
seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;  /* array of seq_t */
    static idx_t  cap   = 0;     /* allocated capacity (elements) */
    static idx_t  len   = 0;     /* number of values currently stored */

    /* Grow the cache if needed */
    if (cap <= n) {
        idx_t new_cap = (n + 1) * 2;          /* simple doubling strategy */

#ifdef __cplusplus
        seq_t *tmp = new (std::nothrow) seq_t[new_cap];
        if (!tmp) abort();                   /* allocation failure */
#else
        /* In pure C, use calloc to zero‑initialize memory */
        seq_t *tmp = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!tmp) abort();                   /* allocation failure */
#endif

        /* Copy existing values */
        for (idx_t i = 0; i < len; ++i) {
            tmp[i] = cache[i];
        }

        /* Free old cache */
#ifdef __cplusplus
        delete [] cache;
#else
        free(cache);
#endif
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute values up to n (inclusive) if not already computed */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;                    /* base case */
        } else {
            seq_t sum  = 0;
            seq_t pow2 = 1;                  /* 2^k */
            for (idx_t k = 0; k < i; ++k) {
                seq_t prod = cache[k] * cache[i - k - 1];
                prod = prod * pow2;
                sum += prod;
                pow2 <<= 1;                  /* next power of two */
            }
            cache[i] = sum + 1;
        }
    }
    len = n + 1;                             /* all indices 0..n are now cached */

    return cache[n];
}
