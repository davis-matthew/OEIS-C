/*
 * A343884  Expansion of e.g.f. exp( x/(1-x)^2 ) / (1-x)^2.
 * https://oeis.org/A343884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n*a(n-1) - 3*(n-1)^2*a(n-2) + (n-2)*(n-1)^2*a(n-3) with
 *            initial values a(0)=1, a(1)=3, a(2)=15
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for idx_t if needed; seq_t is supplied externally */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 15;

    /* Three previous terms */
    seq_t a_nm3 = 1;   /* a(n-3)  */
    seq_t a_nm2 = 3;   /* a(n-2)  */
    seq_t a_nm1 = 15;  /* a(n-1)  */

    for (idx_t i = 3; i <= n; ++i) {
        /* term1 = 3 * i * a_nm1 */
        seq_t term1 = a_nm1 * (seq_t)i * 3;

        /* term2 = 3 * (i-1)^2 * a_nm2 */
        seq_t im1 = (seq_t)(i - 1);
        seq_t term2 = a_nm2 * im1 * im1 * 3;

        /* term3 = (i-2) * (i-1)^2 * a_nm3 */
        seq_t im2 = (seq_t)(i - 2);
        seq_t term3 = a_nm3 * im2 * im1 * im1;

        /* a(i) = term1 - term2 + term3 */
        seq_t a_i = term1 - term2 + term3;

        /* shift the window for the next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }

    return a_nm1;
}
