/*
 * A305355  Expansion of Product_{k>=1} (1 - x^(k*(3*k-1)/2)).
 * https://oeis.org/A305355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 - x^{k(3k-1)/2})
 * Range:     i8 n <= 2811, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"   /* seq_alloc_new_array */
#include "../utils/seq_free_array.h"       /* seq_free_array */

seq_t a(idx_t n)
{
    /* array size n+1, index 0..n */
    size_t N = (size_t)n;
    seq_t *coeff = seq_alloc_new_array(N + 1);
    if (!coeff)
        return 0;                       /* allocation failed */

    /* initialise coefficients: coeff[0] = 1, others 0 */
    for (size_t i = 0; i <= N; ++i)
        coeff[i] = 0;
    coeff[0] = 1;

    /* multiply by each factor (1 - x^{p_k}), p_k = k(3k-1)/2 */
    for (idx_t k = 1; ; ++k) {
        /* compute p_k with 64‑bit arithmetic to avoid overflow */
        uint64_t pk = k * (3 * k - 1) / 2;
        if (pk > (uint64_t)N)
            break;
        size_t ek = (size_t)pk;

        /* update coefficients in descending order */
        for (size_t i = N; i >= ek; --i) {
            coeff[i] -= coeff[i - ek];
            if (i == ek)
                break;                  /* avoid unsigned underflow */
        }
    }

    seq_t result = coeff[N];
    seq_free_array(coeff);
    return result;
}
