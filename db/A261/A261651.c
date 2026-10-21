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
 * Helpers:   coef_dk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/coef_dk.h"

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
        cur  = (seq_t *)realloc(cur,  cur_cap * sizeof(seq_t));
        next = (seq_t *)realloc(next, cur_cap * sizeof(seq_t));
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
