/*
 * A241038  a(n) = A000217(A058481(n)).
 * https://oeis.org/A241038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*3^(2*n) - (3/2)*3^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"   /* provides seq_t pow3(idx_t exp) */

seq_t a(idx_t n)
{
    /* 3^n */
    seq_t t1 = pow3(n);

    /* 3^(2n) = (3^n)^2 */
    seq_t t2 = t1 * t1;

    /* a(n) = (3^(2n) - 3*3^n + 2) / 2 */
    seq_t term = (t2 - (3 * t1) + 2) / 2;

    return term;
}
