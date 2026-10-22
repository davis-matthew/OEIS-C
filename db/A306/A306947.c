/*
 * A306947  Expansion of e.g.f. Product_{k>=0} 1/(1 - x^(2^k)/2^k).
 * https://oeis.org/A306947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of e.g.f. Product_{k>=0} 1/(1 - x^(2^k)/2^k)
   The function a(idx_t n) returns the n-th term of OEIS A306947,
   i.e. a(n) = n! * coefficient of x^n in the product.  */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate arrays for factorials and the current coefficients */
    seq_t *fact = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t *coef = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!fact || !coef) return (seq_t)0;          /* out of memory */

    /* factorial[0] = 1, factorial[i] = i! */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Initial coefficients correspond to the factor 1/(1 - x) :
       a_0(n) = n!  */
    for (idx_t i = 0; i <= n; ++i)
        coef[i] = fact[i];

    /* Process factors for powers of two: step = 2^k, k >= 1 */
    for (idx_t step = 2; step <= n; step <<= 1) {
        /* f represents ((idx)_{step}) / step for the current idx.
           For idx = step we have f = (step-1)! (i.e. fact[step-1])      */
        seq_t f = fact[step - 1];

        /* Update coefficients for all indices >= step */
        for (idx_t idx = step; idx <= n; ++idx) {
            /* Add contribution from this factor */
            seq_t add = f * coef[idx - step];
            coef[idx] += add;

            /* Prepare f for next index if needed */
            if (idx < n) {
                /* f_next = f * (idx+1) / (idx - step + 1)   */
                seq_t num   = f * (seq_t)(idx + 1);
                seq_t denom = (seq_t)(idx + 1 - step);
                f = num / denom;          /* exact division */
            }
        }
    }

    /* Result stored in coef[n] */
    seq_t result = coef[n];
    free(fact);
    free(coef);
    return result;
}
