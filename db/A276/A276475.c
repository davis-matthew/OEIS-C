/*
 * A276475  a(n) = ((sqrt(2); sqrt(2))_n - (-sqrt(2);
 *          -sqrt(2))_n)/(2*sqrt(2)), where (q; q)_n is the q-Pochhammer
 *          symbol.
 * https://oeis.org/A276475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ((sqrt(2); sqrt(2))_n - (-sqrt(2);
 *            -sqrt(2))_n)/(2*sqrt(2))
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 10, i64 n <= 15, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t R = 1;   /* rational part of (sqrt(2);sqrt(2))_n */
    seq_t A = 0;   /* coefficient of sqrt(2) in that product */
    seq_t s = 1;   /* s_k such that (sqrt(2))^k = s + t*sqrt(2) */
    seq_t t = 0;   /* t_k */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update (sqrt(2))^k:  (sqrt(2))^{k} = 2*t_{k-1} + s_{k-1}*sqrt(2)   */
        seq_t ns = t << 1;   /* 2 * t */
        seq_t nt = s;
        s = ns;
        t = nt;

        if ((k & 1) == 0)   /* even k : t == 0, s > 0  */
        {
            seq_t factor = 1 - s;   /* negative integer */
            R *= factor;
            A *= factor;
        }
        else                /* odd k : s == 0, t > 0   */
        {
            /* (sqrt(2))^k = -t*sqrt(2) + 1 */
            /* Update R and A using the recurrence derived above */
            seq_t newR = R - ( (seq_t)2 * A * t );
            seq_t newA = - (R * t) + A;
            R = newR;
            A = newA;
        }
    }
    return A;   /* coefficient of sqrt(2) is the desired value a(n) */
}
