/*
 * A326324  a(n) = A_{5}(n) where A_{m}(x) are the Eulerian polynomials as
 *          defined in A326323.
 * https://oeis.org/A326324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * 4^(k-1) * a(n-k), a(0) = 1.
 *            This implements the recurrence from OEIS A326324
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *a_cache = NULL;  /* cache of a values                */
    static seq_t *pow_cache = NULL;/* 4^k values                       */
    static size_t cap = 0;         /* current array capacity           */
    static idx_t max_n = 0;        /* highest n already computed       */
    static int initialized = 0;    /* flag for first initialisation   */

    /* initialise the first element(s) on the first call */
    if (!initialized) {
        cap = 1;
        a_cache = (seq_t *)calloc(cap, sizeof(seq_t));
        pow_cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!a_cache || !pow_cache) return (seq_t)0;   /* allocation failed */
        a_cache[0] = (seq_t)1;     /* a(0) = 1 */
        pow_cache[0] = (seq_t)1;   /* 4^0 = 1 */
        initialized = 1;
        max_n = 0;
    }

    /* expand cache if necessary */
    if (n >= (idx_t)cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp;

        tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        a_cache = tmp;

        tmp = (seq_t *)realloc(pow_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        pow_cache = tmp;

        /* initialise the new part to zero */
        for (size_t i = cap; i < new_cap; ++i) {
            a_cache[i] = (seq_t)0;
            pow_cache[i] = (seq_t)0;
        }
        cap = new_cap;
    }

    /* if the requested value has already been computed, just return it */
    if (n <= max_n) {
        return a_cache[(size_t)n];
    }

    /* compute any missing values up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* ensure powers of 4 are ready */
        if ((size_t)i >= cap) { /* safety: should not happen after resize */
            pow_cache[i] = pow_cache[i - 1] * (seq_t)4;
        } else {
            if (i > 0)
                pow_cache[(size_t)i] = pow_cache[(size_t)i - 1] * (seq_t)4;
        }

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = binomial((idx_t)i, k);            /* C(i,k)  */
            term = term * pow_cache[(size_t)k - 1];        /* 4^(k-1)  */
            term = term * a_cache[(size_t)i - k];         /* a(i-k)   */
            sum = sum + term;
        }
        a_cache[(size_t)i] = sum;
    }
    max_n = n;

    return a_cache[(size_t)n];
}
