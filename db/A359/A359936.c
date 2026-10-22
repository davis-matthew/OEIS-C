/*
 * A359936  Expansion of Product_{k>=0} (1 - x^(k^2+1)) in powers of x.
 * https://oeis.org/A359936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 627, i16 n <= 2905, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Product Expansion Method for A359936
 *  a(0) = 1
 *  For n > 0, a(n) is the coefficient of x^n in
 *      ∏_{k≥0} (1 – x^{k^2+1}).
 *  This is computed by a DP that processes each factor (1–x^m)
 *  with m = k^2+1 ≤ n, updating the coefficient array from high
 *  to low indices:
 *         coef[i] ← coef[i] – coef[i – m].
 *  The array is of type seq_t, indexed by idx_t (uint64_t).  A
 *  zero‑initialized buffer is obtained via calloc (cast to
 *  seq_t* for C++), which is safe for all supported seq_t types.
 *
 *  The implementation is O(n·√n) and works for any n ≥ 0
 *  regardless of call order, because it recomputes the array
 *  each call.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* Allocate a zero‑initialized array of seq_t of size n+1 */
    seq_t *coef = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!coef) return (seq_t)0;           /* allocation failure → 0 */

    coef[0] = (seq_t)1;                    /* empty product contributes 1 */

    /* Iterate over all exponents m = k^2+1 that are ≤ n */
    for (idx_t k = 0; ; ++k) {
        idx_t m = k * k + 1;               /* exponent of the factor */
        if (m > n) break;                  /* beyond needed range */

        /* Update coefficients from high to low to avoid
         * re‑using updated values in the same iteration.   */
        for (idx_t i = n; i + 1 > m; --i) {        /* i ≥ m */
            coef[i] = coef[i] - coef[i - m];
        }
    }

    seq_t result = coef[n];
    free(coef);
    return result;
}
