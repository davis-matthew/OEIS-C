/*
 * A347274  a(n) = Sum_{j=1..n} j*n^(n+1-j).
 * https://oeis.org/A347274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} j * n^(n+1-j) (OEIS A347274)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* The formula can also be written as
       a(n) = (n^n - n) * n^2 / (n-1)^2   for n > 1, a(1) = 1.
       For arbitrary precision types we avoid intermediate overflow
       by building the powers of n incrementally from the smallest
       exponent upward.  This uses only integer arithmetic.               */

    if (n == 0) return 0;   /* undefined by OEIS, but safe guard */

    seq_t sum   = 0;
    seq_t power = 1;          /* will hold n^k for current k   */
    seq_t sn    = (seq_t)n;   /* cache n as seq_t               */

    for (idx_t k = 1; k <= n; ++k)
    {
        power = seq_mul(power, sn);                    /* n^k          */
        seq_t j = sn + (seq_t)1 - (seq_t)k;            /* j = n+1-k    */
        seq_t term = seq_mul(j, power);                /* j * n^k      */
        sum += term;                                   /* accumulate   */
    }

    return sum;
}
