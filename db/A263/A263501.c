/*
 * A263501  Expansion of phi(-x) * f(-x^2)^3 / f(-x^3) in powers of x where
 *          phi(), f() are Ramanujan theta functions.
 * https://oeis.org/A263501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 546, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Sequence A263501
 *
 * a(q) = q^(-1/8) * η(q)^2 * η(q^2)^2 / η(q^3)
 *    = ∏_{k≥1} (1 - q^k)^{2 + 2*δ_even(k) - δ_3(k)}
 *
 * Here δ_even(k)=1 if k is even, 0 otherwise; δ_3(k)=1 if k is a multiple of 3,
 *       0 otherwise.  The coefficient a(n) is the coefficient of q^n in this
 *       infinite product.
 *
 * The function computes a(n) by maintaining the coefficients of the product
 * up to the largest n requested.  When a larger n arrives, the entire product
 * is recomputed up to that n.  This guarantees correctness with O(n^2)
 * arithmetic for the largest n requested.
 */

static seq_t *coeff = NULL;      /* cached coefficients up to max_n          */
static idx_t  max_n  = 0;        /* largest n for which coeff[0..max_n] exist */
static int    initialized = 0;   /* flag to avoid re‑initialising coeff      */

seq_t a(idx_t n)
{
    /* Initialise on first call.  coeff[0] = 1 for the empty product. */
    if (!initialized) {
        coeff = (seq_t *)calloc(1, sizeof(seq_t));
        coeff[0] = 1;
        max_n  = 0;
        initialized = 1;
    }

    /* Recompute all coefficients if a larger n is requested. */
    if (n > max_n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_coeff = (seq_t *)calloc(new_size, sizeof(seq_t));
        if (!new_coeff) {        /* out of memory – unlikely in test harness */
            return 0;
        }
        new_coeff[0] = 1;        /* coefficient of q^0 */

        /* Build the product ∏_{k=1}^{n} (1 - q^k)^{e(k)} */
        for (idx_t k = 1; k <= n; ++k) {
            int e = 2 + ((k & 1U) == 0 ? 2 : 0) - (k % 3 == 0 ? 1 : 0);
            for (int rep = 0; rep < e; ++rep) {
                for (idx_t i = n; i >= k; --i) {
                    new_coeff[i] = new_coeff[i] - new_coeff[i - k];
                }
            }
        }

        /* Replace old cache and update maximum index. */
        free(coeff);
        coeff = new_coeff;
        max_n = n;
    }

    return coeff[n];
}
