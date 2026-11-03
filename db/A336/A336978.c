/*
 * A336978  Expansion of Product_{k>=1} (1 - x^k * (1 + k*x)).
 * https://oeis.org/A336978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Expansion of Product_{k>=1} (1 - x^k * (1 + k*x)) Defines
 *            a(idx_t n) computing the coefficient of x^n in ∏_{k≥1} (1 - x^k
 *            - k x^{k+1}) truncated to degree n
 * Range:     i8 n <= 17, i16 n <= 36, i32 n <= 87, i64 n <= 218,
 *            i128 n <= 562, big any n
 * Validated: standard, pass, 64 terms through n = 993 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /* static state: coefficients of the product up to processed k */
    static seq_t *coeff   = NULL;   /* array of size coeff_cap */
    static idx_t coeff_cap = 0;     /* current allocated capacity */
    static idx_t processed_k = 0;   /* highest k for which product was updated */

    /* Ensure enough storage for degrees 0..n */
    if ((idx_t)(n + 1) > coeff_cap) {
        /* Free old array and allocate a new one */
        if (coeff) free_seq_array(coeff);
        coeff = alloc_seq_array((size_t)(n + 1));
        coeff_cap = n + 1;
        /* initialise to zero and set the constant term to 1 */
        for (idx_t i = 0; i < coeff_cap; ++i) coeff[i] = 0;
        coeff[0] = 1;
        processed_k = 0;                /* restart computation */
    }

    /* Incrementally multiply by (1 - x^k - k x^{k+1}) for k > processed_k */
    for (idx_t k = processed_k + 1; k <= n; ++k) {
        /* update coefficients in place from high degree downwards */
        for (idx_t i = n; ; --i) {
            seq_t val = coeff[i];
            if (i >= k)          val -= coeff[i - k];
            if (i >= k + 1) {
                seq_t kk = (seq_t)k;
                val -= kk * coeff[i - k - 1];
            }
            coeff[i] = val;
            if (i == 0) break;   /* avoid underflow of unsigned idx_t */
        }
    }

    processed_k = n;
    return coeff[n];
}
