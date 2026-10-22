/*
 * A341092  Rows of Pascal's triangle which contain a 3-term arithmetic
 *          progression of a certain form: a(n) = (2*n^2 + 22*n + 37 + (2*n +
 *          3)*(-1)^n)/8.
 * https://oeis.org/A341092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 + 22*n + 37 + (2*n + 3)*(-1)^n) / 8
 * Range:     u8 n <= 5, u16 n <= 506, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n < 1, safety */

    seq_t tn   = (seq_t)n;
    seq_t part = (seq_t)2 * tn * tn + (seq_t)22 * tn + (seq_t)37;

    seq_t diff = (seq_t)(2 * tn + 3);

    if ((n & 1) == 0)                  /* n even */
        part += diff;                  /* plus (2*n+3) * (+1) */
    else
        part -= diff;                  /* minus (2*n+3) * (-1) */

    return part / 8;
}
