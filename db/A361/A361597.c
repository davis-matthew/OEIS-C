/*
 * A361597  Expansion of e.g.f. exp( x^3/(6 * (1-x)^3) ) / (1-x).
 * https://oeis.org/A361597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   2*a(n) = 2*(4*n-3)*a(n-1) * - 6*(n-1)*(2*n-3)*a(n-2) * +
 *            (n-2)*(n-1)*(8*n-17)*a(n-3) * - 2*(n-3)^2*(n-2)*(n-1)*a(n-4); *
 *            Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=7
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)7;

    /* Maintain the last four values of the sequence */
    seq_t a0 = (seq_t)1; /* a(n-4) */
    seq_t a1 = (seq_t)1; /* a(n-3) */
    seq_t a2 = (seq_t)2; /* a(n-2) */
    seq_t a3 = (seq_t)7; /* a(n-1) */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute the numerator of 2*a(i) */
        seq_t term1 = (seq_t)2 *
                      (((seq_t)4 * i) - (seq_t)3) *
                      a3; /* a(i-1) */

        seq_t term2 = (seq_t)6 *
                      (i - 1) *
                      (((seq_t)2 * i) - (seq_t)3) *
                      a2; /* a(i-2) */

        seq_t term3 = ((seq_t)(i - 2) *
                       (i - 1) *
                       (((seq_t)8 * i) - (seq_t)17)) *
                      a1; /* a(i-3) */

        seq_t term4 = (seq_t)2 *
                      ((seq_t)(i - 3) * (i - 3) *
                       (i - 2) * (i - 1)) *
                      a0; /* a(i-4) */

        seq_t numer = term1 - term2 + term3 - term4;
        ai = numer / (seq_t)2; /* divide by 2 to get a(i) */

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
