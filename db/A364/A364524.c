/*
 * A364524  Expansion of e.g.f. (1-x)^3/(1-3x+2x^2-x^3).
 * https://oeis.org/A364524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 0, a(2) = 2, a(3) = 18 For n >= 4: a(n) =
 *            3*n*a(n-1) - 2*n*(n-1)*a(n-2) + n*(n-1)*(n-2)*a(n-3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 18;

    /* Use the recurrence iteratively, keeping only the last three terms. */
    seq_t a_nm3 = 0;        /* a(1)   */
    seq_t a_nm2 = 2;        /* a(2)   */
    seq_t a_nm1 = 18;       /* a(3)   */
    seq_t an    = 0;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t term1 = 3 * (seq_t)i * a_nm1;
        seq_t term2 = 2 * (seq_t)i * (seq_t)(i - 1);
        term2 *= a_nm2;
        seq_t term3 = (seq_t)i * (seq_t)(i - 1);
        term3 *= (seq_t)(i - 2);
        term3 *= a_nm3;

        an = term1 - term2 + term3;

        /* shift the window for the next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return an;
}
