/*
 * A261651  Expansion of Product_{k>=0} ((1+x^(3*k+1))/(1-x^(3*k+1)))^3.
 * https://oeis.org/A261651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0}
 *            ((1+x^(3k+1))/(1-x^(3k+1)))^3
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 37 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* helper to compute the coefficient d_k for a given m (here m=1) */
static inline seq_t coef_dk(idx_t k)
{
    if (k == 0) return 1;          /* binom(2,2) = 1            */
    if (k == 1) return 6;          /* 3*binom(2,2) + binom(2,2)=6 */
    if (k == 2) return 18;         /* 3*binom(3,2)+binom(3,2)+binom(2,2)=18 */
    /* for k ≥ 3 : d_k = 4k² + 2 */
    return (seq_t)(4 * k * k + 2);
}

seq_t a(idx_t n)
{
    size_t len = (size_t)n + 1U;

    /* static arrays for dynamic programming */
    static size_t cur_cap = 0U;
    static seq_t *cur  = NULL;
    static seq_t *next = NULL;

    /* allocate / grow arrays if needed */
    if (len > cur_cap) {
        cur_cap = len;
        cur  = mem_resize(seq_t, cur, cur_cap);
        next = mem_resize(seq_t, next, cur_cap);
        /* in case realloc moved memory, zero the new part */
    }

    /* initialise current array */
    for (size_t i = 0U; i < len; ++i) cur[i] = 0;
    cur[0] = 1;

    for (idx_t m = 1; m <= n; m += 3) {
        /* zero the next array */
        for (size_t i = 0U; i < len; ++i) next[i] = 0;

        /* convolve cur with the coefficients for this m */
        for (idx_t e = 0; e <= n; ++e) {
            seq_t ce = cur[(size_t)e];
            if (ce == 0) continue;

            idx_t t = e;   /* target exponent starts at e */
            idx_t k = 0;   /* the index in d_k */

            while (t <= n) {
                seq_t dk = coef_dk(k);
                next[(size_t)t] += ce * dk;
                ++k;
                t += m;     /* next exponent where this m contributes */
            }
        }

        /* swap pointers for next iteration */
        seq_t *tmp = cur;
        cur      = next;
        next     = tmp;
    }

    return cur[(size_t)n];
}
