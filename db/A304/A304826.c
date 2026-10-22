/*
 * A304826  a(n) = 32*7^n/21 - 8/3, n>=1.
 * https://oeis.org/A304826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 7*a(n-2), a(1) = 8, a(2) = 72
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined at offset; safety */

    if (n == 1)
        return (seq_t)8;          /* a(1) */
    if (n == 2)
        return (seq_t)72;         /* a(2) */

    /* iterate using the recurrence */
    seq_t a_nm2 = (seq_t)8;       /* a(n-2)   */
    seq_t a_nm1 = (seq_t)72;      /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t a_i = (seq_t)8 * a_nm1 - (seq_t)7 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }

    return a_nm1;                 /* a(n)     */
}
