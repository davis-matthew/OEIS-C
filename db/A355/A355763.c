/*
 * A355763  E.g.f. satisfies A(x)^2 * log(A(x)) = exp(x*A(x)) - 1.
 * https://oeis.org/A355763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A355763
 * Formula: a(n) = Σ_{k=0..n} (n-2*k+1)^(k-1) * Stirling2(n,k)
 */

#include <stdlib.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Static cache for Stirling numbers and sequence values */
    static seq_t *S = NULL;          /* Stirling numbers for current n-1 */
    static size_t Scap = 0;          /* Allocated capacity of S */
    static seq_t *cache = NULL;      /* Cached a(n) values */
    static size_t cachcap = 0;       /* Allocated capacity of cache */
    static idx_t computed = 0;       /* Highest n for which S and cache are valid */

    /* Ensure cache has space for n */
    if (n + 1 > cachcap) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cachcap = n + 1;
    }

    /* Base case: a(0) = 1 */
    if (n == 0) {
        if (cachcap == 0) {
            cache = (seq_t *)realloc(cache, sizeof(seq_t));
            cachcap = 1;
        }
        cache[0] = (seq_t)1;
        return cache[0];
    }

    /* Ensure Stirling array is initialized for n=1 */
    if (computed == 0) {
        S = (seq_t *)realloc(S, sizeof(seq_t));
        Scap = 1;
        S[0] = (seq_t)1;   /* S(0,0) = 1 */
    }

    /* Compute successive rows up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* Allocate space for new row */
        if ((i + 1) > Scap) {
            S = (seq_t *)realloc(S, (i + 1) * sizeof(seq_t));
            Scap = i + 1;
        }

        /* Update Stirling numbers in-place (backward to avoid overwrite) */
        for (idx_t k = i; k >= 1; --k) {
            seq_t term1 = S[k] * (seq_t)k;   /* k * S(n-1,k) */
            seq_t term2 = S[k - 1];          /* S(n-1,k-1) */
            S[k] = term1 + term2;
        }
        S[0] = (seq_t)0;   /* S(n,0) = 0 for n>0 */

        /* Compute a(i) from the freshly-computed Stirling numbers */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t base = (seq_t)i - (seq_t)(2 * k) + (seq_t)1;   /* (n-2k+1) */
            seq_t pow  = seq_pow(base, k - 1);                   /* (n-2k+1)^(k-1) */
            seq_t term = pow * S[k];                             /* term */
            sum = sum + term;
        }
        cache[i] = sum;
    }

    computed = n;
    return cache[n];
}
