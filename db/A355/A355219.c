/*
 * A355219  a(n) = Sum_{k>=1} (4*k - 2)^n / 2^k.
 * https://oeis.org/A355219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + Σ_{k=1..n} binom(n,k) * 4^k * a(n-k) Offset: 0
 *            (a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   seq_realloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation lazily computes a(n) for any n ≥ 0.
   It stores previously computed values in static arrays so that
   subsequent calls with arbitrary n are efficient.  No floating
   point arithmetic is used; all computations are done with
   seq_t arithmetic. */

#include <stdlib.h>   /* malloc, realloc, free, size_t */
#include "../utils/seq_realloc_arr.h"

/* Main function as requested */
seq_t a(idx_t n)
{
    /* Static caches */
    static seq_t *a_cache   = NULL;
    static seq_t *pow2_cache = NULL;
    static seq_t *pow4_cache = NULL;
    static idx_t  cap        = 0;   /* number of elements currently allocated */

    /* Ensure capacity for n+1 elements */
    if (n + 1 > cap) {
        idx_t   new_cap = n + 1;
        idx_t   old_cap = cap;

        /* Resize arrays */
        a_cache   = seq_realloc_arr(a_cache,   old_cap, new_cap);
        pow2_cache= seq_realloc_arr(pow2_cache,old_cap, new_cap);
        pow4_cache= seq_realloc_arr(pow4_cache,old_cap, new_cap);

        /* Initialise the first element if arrays were just created */
        if (old_cap == 0) {
            a_cache[0]    = (seq_t)1;
            pow2_cache[0] = (seq_t)1;
            pow4_cache[0] = (seq_t)1;
            old_cap = 1;
        }

        /* Compute new entries */
        for (idx_t i = old_cap; i < new_cap; ++i) {
            /* pow2[i] = 2^i, pow4[i] = 4^i */
            pow2_cache[i] = pow2_cache[i-1] * (seq_t)2;
            pow4_cache[i] = pow4_cache[i-1] * (seq_t)4;

            /* Compute a[i] using the recurrence
               a[i] = 2^i + Σ_{k=1..i} C(i,k) * 4^k * a[i-k] */
            seq_t sum   = 0;
            seq_t binom = 1;            /* binom(i,0) */
            for (idx_t k = 1; k <= i; ++k) {
                /* Update binom = C(i,k) */
                binom = (binom * (seq_t)(i - k + 1)) / (seq_t)k;
                /* term = binom * 4^k * a[i-k] */
                seq_t term = binom;
                term *= pow4_cache[k];
                term *= a_cache[i - k];
                sum += term;
            }
            a_cache[i] = pow2_cache[i] + sum;
        }
        cap = new_cap;
    }

    return a_cache[n];
}
