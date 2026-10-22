/*
 * A349564  Dirichlet convolution of A011782 [2^(n-1)] with A349450 [Dirichlet
 *          inverse of right-shifted Catalan numbers].
 * https://oeis.org/A349564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} 2^(d-1) * A349450(n/d)
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 17, i64 n <= 36, i128 n <= 68,
 *            big any n
 * Helpers:   mult128, mult_divide, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dirichlet convolution of 2^(n-1) with the Dirichlet inverse of the
   right‑shifted Catalan numbers.  */
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/mult128.h"
#include "../utils/mult_divide.h"

/* Cached sequences.  Indices are 1‑based for a and g; 0‑based for
   the Catalan numbers (Catalan_0 is stored at c_arr[0]). */
static seq_t *g_arr = NULL;      /* Dirichlet inverse terms, 1‑indexed */
static seq_t *c_arr = NULL;      /* Catalan numbers, 0‑indexed */
static seq_t *a_arr = NULL;      /* result sequence, 1‑indexed */
static seq_t *pow2_arr = NULL;   /* 2^(n-1), 1‑indexed */
static idx_t last_cap = 0;       /* maximum n computed so far */

/* Extend precomputation up to new_cap (inclusive).  */
static void extend(idx_t new_cap)
{
    if (new_cap <= last_cap)
        return;

    /* Allocate or grow storage.  */
    if (last_cap == 0) {
        g_arr    = seq_alloc(new_cap + 1);
        a_arr    = seq_alloc(new_cap + 1);
        c_arr    = seq_alloc(new_cap);   /* Catalan indices 0..new_cap-1 */
        pow2_arr = seq_alloc(new_cap + 1);
    } else {
        g_arr    = seq_realloc(g_arr,    last_cap + 1, new_cap + 1);
        a_arr    = seq_realloc(a_arr,    last_cap + 1, new_cap + 1);
        c_arr    = seq_realloc(c_arr,    last_cap,     new_cap);
        pow2_arr = seq_realloc(pow2_arr, last_cap + 1, new_cap + 1);
    }

    /* Initialize Catalan_0 */
    if (last_cap == 0)
        c_arr[0] = (seq_t)1;

    /* Precompute values up to new_cap.  */
    for (idx_t idx = last_cap + 1; idx <= new_cap; ++idx) {
        /* Catalan numbers.  */
        if (idx > 1) {
            idx_t i = idx - 1;
            /* c_arr[i] = c_arr[i-1] * (4*i-2) / (i+1)   (exact division) */
            c_arr[i] = mult_divide(c_arr[i - 1], (seq_t)(4 * i - 2), i + 1);
        }

        /* Powers of two.  */
        if (idx == 1)
            pow2_arr[1] = (seq_t)1;
        else
            pow2_arr[idx] = mult128(pow2_arr[idx - 1], (seq_t)2);

        /* Dirichlet inverse g_arr[idx].  */
        if (idx == 1) {
            g_arr[1] = (seq_t)1;
        } else {
            seq_t sum = 0;
            for (idx_t k = 1; k * k <= idx; ++k) {
                if (idx % k == 0) {
                    idx_t d1 = k;
                    idx_t d2 = idx / k;

                    if (d1 > 1) {                /* divisor d1 > 1 */
                        seq_t term = mult128(c_arr[d1 - 1], g_arr[idx / d1]);
                        sum += term;
                    }
                    if (d2 != d1 && d2 > 1) {    /* divisor d2 > 1, distinct */
                        seq_t term = mult128(c_arr[d2 - 1], g_arr[idx / d2]);
                        sum += term;
                    }
                }
            }
            g_arr[idx] = -sum;
        }

        /* Compute a_arr[idx] = Σ_{d|idx} 2^(d-1) * g_arr[idx/d].  */
        seq_t sum_a = 0;
        for (idx_t k = 1; k * k <= idx; ++k) {
            if (idx % k == 0) {
                idx_t d1 = k;
                idx_t d2 = idx / k;

                /* divisor d1 */
                seq_t term1 = mult128(pow2_arr[d1], g_arr[idx / d1]);
                sum_a += term1;

                /* paired divisor d2 (if distinct) */
                if (d2 != d1) {
                    seq_t term2 = mult128(pow2_arr[d2], g_arr[idx / d2]);
                    sum_a += term2;
                }
            }
        }
        a_arr[idx] = sum_a;
    }

    last_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    extend(n);
    return a_arr[n];
}
