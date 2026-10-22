/*
 * A368466  a(n) = Sum_{k=0..n} 2^k * k^n.
 * https://oeis.org/A368466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 2^k * k^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;         /* running total of a(n)      */
    seq_t pow2 = 1;        /* current value of 2^k       */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term;
        /* handle 0^0 = 1, otherwise 0^n = 0 for n>0 */
        if (k == 0)
        {
            term = (n == 0) ? 1 : 0;
        }
        else
        {
            term = seq_pow((seq_t)k, n);   /* k^n          */
        }

        term = term * pow2;      /* 2^k * k^n     */
        sum = sum + term;        /* accumulate    */

        pow2 = pow2 * ((seq_t)2);   /* advance 2^(k+1) */
    }

    return sum;
}
