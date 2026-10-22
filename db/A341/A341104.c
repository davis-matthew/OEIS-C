/*
 * A341104  a(n) = [x^n] (x - 1)^4/((1 - 2*x)*(x^2 - 3*x + 1)).
 * https://oeis.org/A341104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 2*a(n-3) with initial values
 *            a(0)=1, a(1)=1, a(2)=4, a(3)=11, a(4)=30
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)11;
    if (n == 4) return (seq_t)30;

    seq_t a_minus3 = (seq_t)4;   /* a(n-3) when n==5 */
    seq_t a_minus2 = (seq_t)11;  /* a(n-2) when n==5 */
    seq_t a_minus1 = (seq_t)30;  /* a(n-1) when n==5 */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)5 * a_minus1
                     - (seq_t)7 * a_minus2
                     + (seq_t)2 * a_minus3;
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = next;
    }

    return a_minus1;
}
