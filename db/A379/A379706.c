/*
 * A379706  Number of nonempty labeled antichains of subsets of [n] such that
 *          the largest subset is of size 2.
 * https://oeis.org/A379706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n-2} binomial(n,i) * (2^binomial(n-i,2) - 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* a(0)=0, a(1)=0 */

    seq_t sum   = (seq_t)0;
    seq_t comb  = (seq_t)1;              /* binomial(n,0) */
    const seq_t base_two = (seq_t)2;

    for (idx_t i = 0; i <= n - 2; ++i)
    {
        idx_t m   = n - i;                              /* m = n-i */
        idx_t exp = m * (m - 1) / 2;                    /* C(m,2) */

        seq_t term = big_pow(base_two, (seq_t)exp);     /* 2^exp */
        term -= (seq_t)1;                               /* 2^exp - 1 */

        sum += comb * term;                             /* add term */

        if (i < n - 2)                                  /* update comb to binomial(n,i+1) */
        {
            comb = comb * (seq_t)(n - i) / (seq_t)(i + 1);
        }
    }

    return sum;
}
