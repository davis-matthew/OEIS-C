/*
 * A267044  Binary representation of the middle column of the "Rule 91"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + a(n-2) – 10*a(n-3) for n>4 Initial values:
 *            a(0)=1, a(1)=10, a(2)=100, a(3)=1000, a(4)=10001
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)100;
    if (n == 3) return (seq_t)1000;
    if (n == 4) return (seq_t)10001;

    /* Use the recurrence for n >= 5 */
    seq_t a_nm3 = (seq_t)100;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)1000;  /* a(n-2)  */
    seq_t a_nm1 = (seq_t)10001; /* a(n-1)  */
    seq_t a_n = (seq_t)0;

    for (idx_t k = 5; k <= n; ++k) {
        a_n = a_nm1 * (seq_t)10 + a_nm2 - a_nm3 * (seq_t)10;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
