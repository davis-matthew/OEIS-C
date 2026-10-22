/*
 * A354395  Expansion of e.g.f. exp( -(exp(x) - 1)^2 / 2 ).
 * https://oeis.org/A354395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{k=1..n} binomial(n-1,k-1) * Stirling2(k,2) * a(n-k)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 14, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* With Stirling2(k,2) = 2^{k-1} – 1 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /*----- Memoisation cache for a(n) -----*/
    static seq_t *cache = NULL;       /* array of computed a values   */
    static idx_t cache_cap = 0;       /* capacity of the array        */
    static idx_t max_computed = 0;    /* highest index computed in cache */

    /* Expand cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (n + 1) > 2 ? (n + 1) : 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /*----- Cache of powers of 2 (as Stirling2(k,2)+1) -----*/
    static seq_t *pow2 = NULL;          /* pow2[i] = 2^i   */
    static idx_t pow2_cap = 0;

    /* we need powers up to n-1 (for k‑1 where k ≤ n) */
    if (n >= pow2_cap) {
        idx_t new_cap = (n + 1) > 2 ? (n + 1) : 2;
        seq_t *tmp = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
        pow2 = tmp;
        if (pow2_cap == 0) {
            pow2[0] = (seq_t)1;
            pow2_cap = 1;
        }
        for (idx_t i = pow2_cap; i < new_cap; ++i) {
            pow2[i] = pow2[i-1] * (seq_t)2;
        }
        pow2_cap = new_cap;
    }

    /* Initialise the first term a(0) if not yet done */
    if (max_computed == 0 && cache_cap > 0) {
        cache[0] = (seq_t)1;
    }

    /* Compute values up to requested n if needed */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t bin  = binomial(i - 1, k - 1);      /* C(i-1,k-1)   */
                seq_t s    = pow2[k - 1];                 /* 2^{k-1}      */
                s -= (seq_t)1;                            /* –1 → S(k,2)  */
                seq_t term = bin;
                term *= s;
                term *= cache[i - k];
                sum += term;
            }
            cache[i] = -sum;  /* a(i) = -sum */
        }
        max_computed = n;
    }

    return cache[n];
}
