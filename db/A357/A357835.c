/*
 * A357835  a(n) = Sum_{k=0..floor((n-1)/3)} Stirling1(n,3*k+1).
 * https://oeis.org/A357835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..⌊(n-1)/3⌋} Stirling1(n,3*k+1)
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 13, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* a(0) = 0 */

    /* R_1(n) = sum_{k≡1 (mod 3)} S(n,k)  →  a(n)  */
    /* R_0(n) and R_2(n) are auxiliary sums for the recurrence  */
    seq_t A = 0;    /* R_1 */
    seq_t B = 1;    /* R_0 (S(0,0)=1) */
    seq_t C = 0;    /* R_2 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t coef = (seq_t)(i - 1);
        seq_t newA = B - coef * A;
        seq_t newB = C - coef * B;
        seq_t newC = A - coef * C;
        A = newA;
        B = newB;
        C = newC;
    }

    return A;      /* a(n) is R_1(n) */
}
