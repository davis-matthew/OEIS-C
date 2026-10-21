/*
 * A294189  E.g.f.: exp(2*(1/(1-x)^2 - 1)).
 * https://oeis.org/A294189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n+1)*a(n-1) - 3*(n-2)*(n-1)*a(n-2) +
 *            (n-3)*(n-2)*(n-1)*a(n-3) with a(0)=1, a(1)=4, a(2)=28
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* handle the first three terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)28;

    /* a_nm3 = a(i-3), a_nm2 = a(i-2), a_nm1 = a(i-1) */
    seq_t a_nm3 = (seq_t)1;      /* a(0) */
    seq_t a_nm2 = (seq_t)4;      /* a(1) */
    seq_t a_nm1 = (seq_t)28;     /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        /* compute the coefficients */
        idx_t c1 = 3 * i + 1;
        idx_t c2 = 3 * (i - 2) * (i - 1);
        idx_t c3 = (i - 3) * (i - 2) * (i - 1);

        /* multiply coefficients with the appropriate previous terms */
        seq_t term1 = (seq_t)c1 * a_nm1;
        seq_t term2 = (seq_t)c2 * a_nm2;
        seq_t term3 = (seq_t)c3 * a_nm3;

        /* res = term1 + term3 - term2, avoiding a negative intermediate */
        seq_t res = term1 + term3;
        res -= term2;

        /* shift values for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = res;
    }

    return a_nm1;
}
