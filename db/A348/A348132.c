/*
 * A348132  a(n) is the denominator of the relativistic sum of n velocities of
 *          1/n, in units where the speed of light is 1.
 * https://oeis.org/A348132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(2n-1) = n^(2n-1) + (n-1)^(2n-1) a(2n) = ((2n+1)^(2n) +
 *            (2n-1)^(2n)) / 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    if (n & 1)   /* n is odd: n = 2m-1 */
    {
        idx_t m  = (n + 1) >> 1;                   /* m = (n+1)/2 */
        seq_t base1 = (seq_t)m;
        seq_t base2 = (seq_t)(m - 1);
        seq_t exp   = (seq_t)(2 * m - 1);

        seq_t p1 = big_pow(base1, exp);
        seq_t p2 = big_pow(base2, exp);
        return p1 + p2;
    }
    else        /* n is even: n = 2m */
    {
        idx_t m  = n >> 1;                        /* m = n/2 */
        seq_t base1 = (seq_t)(2 * m + 1);
        seq_t base2 = (seq_t)(2 * m - 1);
        seq_t exp   = (seq_t)(2 * m);

        seq_t p1 = big_pow(base1, exp);
        seq_t p2 = big_pow(base2, exp);
        return (p1 + p2) >> 1;                    /* divide by 2 */
    }
}
