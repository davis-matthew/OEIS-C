/*
 * A356127  a(n) = Sum_{k=1..n} k^k * binomial(floor(n/k)+1,2).
 * https://oeis.org/A356127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^k * binomial(floor(n/k)+1,2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* offset 1, a(1) is first term */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t pk   = big_pow((seq_t)k, (seq_t)k);        /* k^k            */
        idx_t  m   = n / k + 1;                         /* floor(n/k)+1   */
        seq_t comb = binomial((idx_t)m, (idx_t)2);      /* C(m,2)         */
        sum += pk * comb;                               /* add to running sum */
    }
    return sum;
}
