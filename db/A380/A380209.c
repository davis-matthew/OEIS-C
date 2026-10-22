/*
 * A380209  Expansion of e.g.f. exp( (1+2*x)^(3/2) - 1 ).
 * https://oeis.org/A380209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} 3^k * 2^(n-k) * Stirling1(n,k) * Bell(k)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 19, i128 n <= 22,
 *            big any n
 * Helpers:   seq_realloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the terms by dynamic programming,
   storing the Stirling numbers of the first kind, Bell numbers,
   and the required powers of 3 and 2.  It works for any seq_t
   supplied by the build, including signed, unsigned, and arbitrary‑
   precision integer types. */

#include <stdlib.h>
#include "../utils/seq_realloc_zero.h"

seq_t a(idx_t n)
{
    /* Static storage for all intermediate data.  The arrays are
       enlarged only when a larger n than previously requested is
       encountered. */
    static size_t max_n = 0;    /* highest n computed so far */
    static size_t cap   = 0;    /* current array capacity (elements) */

    static seq_t *a_cache  = NULL;  /* a[0..max_n]              */
    static seq_t *bell     = NULL;  /* Bell[0..max_n]            */
    static seq_t *pow3     = NULL;  /* 3^k for k=0..max_n       */
    static seq_t *pow2     = NULL;  /* 2^k for k=0..max_n       */
    static seq_t *s_prev   = NULL;  /* Stirling1(n-1,k) for k<=n-1 */
    static seq_t *s_curr   = NULL;  /* Stirling1(n,k)   for k<=n   */

    /* Initial allocation for n = 0 */
    if (cap == 0) {
        cap = 1;
        a_cache  = (seq_t *)seq_realloc_zero(NULL, 0, cap);
        bell     = (seq_t *)seq_realloc_zero(NULL, 0, cap);
        pow3     = (seq_t *)seq_realloc_zero(NULL, 0, cap);
        pow2     = (seq_t *)seq_realloc_zero(NULL, 0, cap);
        s_prev   = (seq_t *)seq_realloc_zero(NULL, 0, cap);
        s_curr   = (seq_t *)seq_realloc_zero(NULL, 0, cap);

        /* Base values for n = 0 */
        a_cache[0]  = (seq_t)1;
        bell[0]     = (seq_t)1;
        pow3[0]     = (seq_t)1;
        pow2[0]     = (seq_t)1;
        s_prev[0]   = (seq_t)1;          /* Stirling1(0,0) = 1 */
        max_n = 0;
    }

    /* Grow arrays if necessary to reach the requested n */
    if ((size_t)n > max_n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cap) {
            a_cache = (seq_t *)seq_realloc_zero(a_cache, cap, new_cap);
            bell    = (seq_t *)seq_realloc_zero(bell,    cap, new_cap);
            pow3    = (seq_t *)seq_realloc_zero(pow3,    cap, new_cap);
            pow2    = (seq_t *)seq_realloc_zero(pow2,    cap, new_cap);
            s_prev  = (seq_t *)seq_realloc_zero(s_prev,  cap, new_cap);
            s_curr  = (seq_t *)seq_realloc_zero(s_curr,  cap, new_cap);
            cap = new_cap;
        }

        /* Compute all needed terms from max_n+1 up to n */
        for (size_t new_n = max_n + 1; new_n <= (size_t)n; ++new_n) {
            /* Update powers of 3 and 2 */
            pow3[new_n] = pow3[new_n - 1] * (seq_t)3;
            pow2[new_n] = pow2[new_n - 1] * (seq_t)2;

            /* Compute Stirling numbers of the first kind for current n */
            s_curr[0] = 0;
            for (size_t k = 1; k <= new_n; ++k) {
                seq_t term1 = s_prev[k - 1];
                seq_t term2 = s_prev[k] * (seq_t)(new_n - 1);
                s_curr[k] = term1 - term2;
            }

            /* Compute Bell number for current n using recurrence:
               B(n) = sum_{k=0}^{n-1} binom(n-1,k) * B(k)  */
            seq_t sumB  = 0;
            seq_t binom = 1;                 /* binom(n-1,0) */
            size_t up   = new_n - 1;
            for (size_t k = 0; k <= up; ++k) {
                sumB += binom * bell[k];
                if (k < up) {
                    binom = binom * (seq_t)(up - k) / (seq_t)(k + 1);
                }
            }
            bell[new_n] = sumB;

            /* Compute a(new_n) = sum_{k=0}^{new_n} 3^k * 2^(new_n-k) *
                                 Stirling1(new_n,k) * Bell(k) */
            seq_t sumA = 0;
            for (size_t k = 0; k <= new_n; ++k) {
                sumA += pow3[k] * pow2[new_n - k] * s_curr[k] * bell[k];
            }
            a_cache[new_n] = sumA;

            /* Prepare for next iteration: swap s_prev and s_curr */
            seq_t *tmp = s_prev;
            s_prev = s_curr;
            s_curr = tmp;
        }

        max_n = (size_t)n;
    }

    return a_cache[(size_t)n];
}
