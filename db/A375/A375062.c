/*
 * A375062  Expansion of 1 / Sum_{k in Z} x^k / (1 - x^(5*k+1)).
 * https://oeis.org/A375062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 19, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   compute_up_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) from OEIS A375062
 *  Coefficient of x^n in
 *   ∏_{k>0} ((1 - x^{5k-1}) (1 - x^{5k-4}))^3 /
 *           ((1 - x^k) (1 - x^{5k}))
 *  Implementation uses successive convolution with the factors.
 */

#include <stdlib.h>
#include "../utils/compute_up_to.h"

static seq_t *coeff_cache = NULL;
static idx_t coeff_cache_len = 0;

seq_t a(idx_t n)
{
    if (!coeff_cache || n >= coeff_cache_len) {
        seq_t *new_coeff;
        compute_up_to(n, &new_coeff);
        if (coeff_cache) free(coeff_cache);
        coeff_cache = new_coeff;
        coeff_cache_len = n + 1;
    }
    return coeff_cache[n];
}
