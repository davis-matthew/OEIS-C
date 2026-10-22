/*
 * A300799  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3
 *          or 4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A300799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 3*a(n-2) - 5*a(n-3) with initial values
 *            a(1)=4, a(2)=32, a(3)=228 (OEIS A300799)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 0)
        return 0; /* undefined for n<1, return 0 for safety */

    if (n == 1)
        return (seq_t)4;
    if (n == 2)
        return (seq_t)32;
    if (n == 3)
        return (seq_t)228;

    seq_t a_nm3 = (seq_t)4;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)32;  /* a(n-2)  */
    seq_t a_nm1 = (seq_t)228; /* a(n-1)  */
    seq_t a_n   = 0;          /* a(n)    */

    for (idx_t i = 4; i <= n; ++i) {
        a_n = 7 * a_nm1 + 3 * a_nm2 - 5 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
