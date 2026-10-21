/*
 * A291108  Expansion of Sum_{k>=2} k^2*x^(2*k)/(1 - x^k).
 * https://oeis.org/A291108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of squares of nontrivial divisors of n
 * Range:     u8 n <= 23, u16 n <= 359, u32 n <= 9170, u64 n <= 9170,
 *            u128 n <= 9170, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    a(n) = \displaystyle\sum_{\substack{d\mid n \\ 1<d<n}} d^2            */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 2; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t j = n / i;
            seq_t sq_i = (seq_t)i * (seq_t)i;
            sum += sq_i;
            if (j != i)
            {
                seq_t sq_j = (seq_t)j * (seq_t)j;
                sum += sq_j;
            }
        }
    }
    return sum;
}
