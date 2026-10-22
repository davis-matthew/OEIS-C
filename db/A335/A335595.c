/*
 * A335595  E.g.f.: exp(-x * (2 + x)) / (1 - x)^2.
 * https://oeis.org/A335595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*a(n-1) + 2*(n-1)*(n-2)*a(n-3); with initial values:
 *            a(0)=1, a(1)=0, a(2)=0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first three indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)0;

    /* a(n-3), a(n-2), a(n-1) at the start of the loop */
    seq_t a_nm3 = (seq_t)1;   /* a(0) */
    seq_t a_nm2 = (seq_t)0;   /* a(1) */
    seq_t a_nm1 = (seq_t)0;   /* a(2) */
    seq_t new_val;

    for (idx_t i = 3; i <= n; ++i) {
        /* (n-1)*a(n-1) */
        seq_t term1 = (seq_t)(i - 1) * a_nm1;

        /* 2*(n-1)*(n-2)*a(n-3) */
        seq_t term2 = (seq_t)(i - 1) * (i - 2);   /* (n-1)*(n-2) as seq_t */
        term2 = (seq_t)2 * term2 * a_nm3;         /* multiply by 2 and a(n-3) */

        new_val = term1 + term2;

        /* shift window for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = new_val;
    }

    return a_nm1;
}
