/*
 * A368573  a(n) = Sum_{k=0..n} k! * k^k.
 * https://oeis.org/A368573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k! * k^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"  /* for big_pow(seq_t, idx_t) */
#include "../utils/seq_mul.h"  /* for seq_mul(seq_t, seq_t) */

seq_t a(idx_t n)
{
    seq_t sum  = 0;     /* running total of the series */
    seq_t fact = 1;     /* current factorial k! (0! = 1) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* compute k^k, treating 0^0 as 1 */
        seq_t pow_k = big_pow((seq_t)k, k);

        /* term = k! * k^k */
        seq_t term = seq_mul(fact, pow_k);

        sum += term;

        /* update factorial for next k, unless we are at the last k */
        if (k != n)
            fact = seq_mul(fact, (seq_t)(k + 1));
    }
    return sum;
}
