/*
 * A383225  a(n) = sqrt(1 + P(n)*P(n+1)*P(n+2)*P(n+3)) where P(n) = A000129(n)
 *          are the Pell numbers.
 * https://oeis.org/A383225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = P(n+1)*P(n+2) - (-1)^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pell numbers P defined by P(0)=0, P(1)=1, P(k)=2*P(k-1)+P(k-2) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute Pell numbers P(n+1) and P(n+2) */
    seq_t P_nplus1 = 0;
    seq_t P_nplus2 = 0;

    /* base values for P(0) and P(1) */
    seq_t P_i_2 = 0; /* P(0) */
    seq_t P_i_1 = 1; /* P(1) */

    /* set known values for n+1 and n+2 when they are 0 or 1 */
    if (n + 1 == 0) P_nplus1 = 0;
    else if (n + 1 == 1) P_nplus1 = 1;

    if (n + 2 == 0) P_nplus2 = 0;
    else if (n + 2 == 1) P_nplus2 = 1;

    /* iterate from 2 up to n+2 computing Pell numbers */
    for (idx_t i = 2; i <= n + 2; ++i) {
        seq_t P_i = 2 * P_i_1 + P_i_2; /* recurrence */
        if (i == n + 1) P_nplus1 = P_i;
        if (i == n + 2) P_nplus2 = P_i;
        P_i_2 = P_i_1;
        P_i_1 = P_i;
    }

    seq_t prod = P_nplus1 * P_nplus2;
    /* subtract (-1)^n */
    if ((n & 1) == 0) {
        /* n even: subtract 1 */
        prod -= 1;
    } else {
        /* n odd: add 1 */
        prod += 1;
    }
    return prod;
}
