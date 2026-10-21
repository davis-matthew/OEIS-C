/*
 * A271357  a(n) = k*Fibonacci(2*n+1) + (k+1)*Fibonacci(2*n), where k=3.
 * https://oeis.org/A271357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2), a(0) = 3, a(1) = 10
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the offset cases */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)10;

    /* Use the recurrence: a(n) = 3*a(n-1) - a(n-2) */
    seq_t a_nm2 = (seq_t)3;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)10;  /* a(n-1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)3 * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return a_nm1;
}
