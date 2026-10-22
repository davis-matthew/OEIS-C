/*
 * A362858  Expansion of e.g.f. exp(-3*x) / (1 + LambertW(-x)).
 * https://oeis.org/A362858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-3)^(n-k) * k^k * binomial(n,k)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A362858.  Uses static caching to speed up
   repeated calls.  Only allowed operations and helper functions are
   employed. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"        /* for big_pow(seq_t, seq_t) */

/* Grow a dynamic array of seq_t values to at least ‘need’ elements. */
static void grow_seq_cache(seq_t **arr, size_t *cap_p, size_t need)
{
    if (*cap_p >= need)
        return;
    size_t newcap = (*cap_p == 0) ? 1 : *cap_p * 2;
    while (newcap < need)
        newcap <<= 1;
    *arr = (seq_t*)realloc(*arr, newcap * sizeof(seq_t));
    *cap_p = newcap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static caches for previously computed values. */
    static seq_t *a_cache      = NULL;
    static idx_t  max_a        = (idx_t)-1;
    static size_t cap_a        = 0;

    static seq_t *f_cache      = NULL;   /* k^k */
    static idx_t  max_f        = (idx_t)-1;
    static size_t cap_f        = 0;

    static seq_t *pow3_cache   = NULL;   /* (-3)^i */
    static idx_t  max_pow3     = (idx_t)-1;
    static size_t cap_pow3     = 0;

    /* Ensure arrays can hold indices up to n. */
    grow_seq_cache(&a_cache,   &cap_a,   (size_t)(n + 1));
    grow_seq_cache(&f_cache,   &cap_f,   (size_t)(n + 1));
    grow_seq_cache(&pow3_cache,&cap_pow3,(size_t)(n + 1));

    /* Initialize caches if not yet done. */
    if (max_f == (idx_t)-1) {             /* f_cache[0] = 0^0 = 1 */
        f_cache[0] = (seq_t)1;
        max_f = 0;
    }
    if (max_pow3 == (idx_t)-1) {
        pow3_cache[0] = (seq_t)1;         /* (-3)^0 = 1 */
        max_pow3 = 0;
    }
    if (max_a == (idx_t)-1) {
        a_cache[0] = (seq_t)1;            /* a(0) = 1 */
        max_a = 0;
    }

    /* Precompute (-3)^i up to n. */
    if (n > max_pow3) {
        for (idx_t i = max_pow3 + 1; i <= n; ++i)
            pow3_cache[i] = pow3_cache[i - 1] * (seq_t)(-3);
        max_pow3 = n;
    }

    /* Precompute k^k for k up to n. */
    if (n > max_f) {
        for (idx_t k = max_f + 1; k <= n; ++k) {
            if (k == 0)
                f_cache[k] = (seq_t)1;   /* define 0^0 as 1 */
            else
                f_cache[k] = big_pow((seq_t)k, (seq_t)k);
        }
        max_f = n;
    }

    /* Compute a(n) if it hasn't been computed yet. */
    if (n > max_a) {
        for (idx_t curr = max_a + 1; curr <= n; ++curr) {
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;        /* binom(curr,0) */

            for (idx_t k = 0; k <= curr; ++k) {
                seq_t term = binom * f_cache[k] * pow3_cache[curr - k];
                sum += term;
                if (k < curr) {
                    /* binom(curr,k+1) = binom(curr,k) * (curr-k)/(k+1) */
                    binom = binom * (seq_t)(curr - k) / (seq_t)(k + 1);
                }
            }
            a_cache[curr] = sum;
        }
        max_a = n;
    }

    return a_cache[n];
}
