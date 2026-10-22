/*
 * A366459  Expansion of e.g.f. -log(1 - x^2 * exp(x)).
 * https://oeis.org/A366459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..floor(n/2)} k^(n-2*k-1)/(n-2*k)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = a(1) = 0 */
    if (n < 2)
        return 0;

    seq_t sum = 0;

    /* Initialize P = (n-1)*n = n!/(n-2)!  (k = 1) */
    seq_t P = 1;
    P *= (seq_t)(n - 1);
    P *= (seq_t)n;

    idx_t maxk = n / 2;

    for (idx_t k = 1; k <= maxk; ++k)
    {
        idx_t diff = n - 2 * k;          /* n-2k  ≥ 0 */

        seq_t term;
        if (diff == 0)                   /* exponent = -1 : integer division by k */
        {
            term = P / ((seq_t)k);
        }
        else                             /* exponent = diff - 1  ≥ 0 */
        {
            seq_t exp = (seq_t)(diff - 1);
            term = P * big_pow((seq_t)k, exp);
        }

        sum += term;

        /* Extend the factorial product for the next value of k.
           Multiply by the two new lower factors: n-2k-1 and n-2k. */
        if (k < maxk)
        {
            idx_t f1 = n - 2 * (k + 1) + 1;  /* n - 2k - 1 */
            idx_t f2 = n - 2 * (k + 1) + 2;  /* n - 2k   */
            P *= (seq_t)f1;
            P *= (seq_t)f2;
        }
    }

    return sum;
}
