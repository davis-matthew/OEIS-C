/*
 * A306423  Number of coalescent histories for pseudocaterpillar gene trees G
 *          and caterpillar species trees S.
 * https://oeis.org/A306423
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by the linear recurrence 2*n*a(n) +
 *            (-11*n+18)*a(n-1) + (11*n-38)*a(n-2) + 2*(2*n-11)*a(n-3) = 0 for
 *            n >= 7, with initial values a(4) = 3, a(5) = 11, a(6) = 37
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 4) return (seq_t)3;
    if (n == 5) return (seq_t)11;
    if (n == 6) return (seq_t)37;

    seq_t prev3 = (seq_t)3;   /* a(n-3)  */
    seq_t prev2 = (seq_t)11;  /* a(n-2)  */
    seq_t prev1 = (seq_t)37;  /* a(n-1)  */
    seq_t an = 0;

    for (idx_t k = 7; k <= n; ++k)
    {
        /* compute coefficients as seq_t to avoid overflow */
        seq_t coeff1 = ((seq_t)11 * k) - (seq_t)18;      /* 11*k-18 */
        seq_t coeff2 = ((seq_t)11 * k) - (seq_t)38;      /* 11*k-38 */
        seq_t coeff3 = ((seq_t)2 * ((seq_t)2 * k - (seq_t)11)); /* 2*(2*k-11) */

        seq_t term1 = coeff1 * prev1;   /* (11*k-18)*a(n-1) */
        seq_t term2 = coeff2 * prev2;   /* (11*k-38)*a(n-2) */
        seq_t term3 = coeff3 * prev3;   /* 2*(2*k-11)*a(n-3) */

        seq_t num = term1;
        num -= term2;   /* term1 - term2 */
        num -= term3;   /* - term3 */

        seq_t denom = ((seq_t)2 * k);   /* 2*k */
        an = num / denom;              /* division is exact */

        /* shift state for next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = an;
    }
    return prev1;   /* current value a(n) */
}
