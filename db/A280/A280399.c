/*
 * A280399  Number of 1 X n 0..2 arrays with no element unequal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A280399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula for A280399 (recurrence):
   a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3) - 4*a(n-4)  for n>5
   a(1)=0, a(2)=0, a(3)=2, a(4)=2, a(5)=8
   If called with n==0 this returns 0 for safety. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* safety for input 0 */
        return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)2;
    if (n == 5) return (seq_t)8;

    /* variables for last four terms: a(n-1), a(n-2), a(n-3), a(n-4) */
    seq_t a_nm1 = (seq_t)8;  /* a(5) */
    seq_t a_nm2 = (seq_t)2;  /* a(4) */
    seq_t a_nm3 = (seq_t)2;  /* a(3) */
    seq_t a_nm4 = (seq_t)0;  /* a(2) */

    seq_t cur = 0;

    for (idx_t i = 6; i <= n; ++i) {
        cur = 2 * a_nm1 + 3 * a_nm2 - 4 * a_nm3 - 4 * a_nm4;
        /* shift window for next step */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}
