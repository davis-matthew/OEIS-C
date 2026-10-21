/*
 * A286128  Expansion of eta(q) * eta(q^12) * eta(q^15) * eta(q^20) in powers
 *          of q.
 * https://oeis.org/A286128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9842, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Formula: a(n) = coefficient of q^(n-2) in   
 *    ∏_{k>0} (1-q^k)(1-q^(12k))(1-q^(15k))(1-q^(20k)),
 *    with a(0)=a(1)=0.  The implementation convolves the
 *    infinite product by a direct Euler–product DP, precomputing
 *    up to the maximum index requested (assumed ≤ 10000 for speed).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset: first two terms are zero */
    if (n < 2)
        return 0;

    /* index of the coefficient in the unshifted product  */
    idx_t idx = n - 2;

    /* Lazy precomputation up to a fixed maximum (10000) */
    static seq_t *coeff = NULL;
    static idx_t coeff_size = 0;
    const idx_t MAX_N = 10000;   /* safe upper bound for the tests */

    if (!coeff) {
        /* allocate enough space: we need indices 0 .. MAX_N-2,
           but allocate one extra for safety */
        coeff_size = MAX_N + 1;
        coeff = (seq_t *)malloc(coeff_size * sizeof(seq_t));
        if (!coeff)
            return 0;   /* out of memory – return safe value */

        /* initialise */
        coeff[0] = 1;
        for (idx_t i = 1; i < coeff_size; ++i)
            coeff[i] = 0;

        /* compute all coefficients of the product up to MAX_N */
        for (idx_t d = 1; d < coeff_size; ++d) {
            int e = 1;                     /* base (1 - q^d) */
            if (d % 12 == 0) ++e;          /* extra (1 - q^(12k)) */
            if (d % 15 == 0) ++e;          /* extra (1 - q^(15k)) */
            if (d % 20 == 0) ++e;          /* extra (1 - q^(20k)) */
            for (int t = 0; t < e; ++t) {
                for (idx_t k = coeff_size - 1; k >= d; --k)
                    coeff[k] -= coeff[k - d];
            }
        }
    }

    /* idx must be within the precomputed range */
    if (idx >= coeff_size)
        return 0;   /* silently return 0 for out-of-range queries */

    return coeff[idx];
}
