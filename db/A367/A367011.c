/*
 * A367011  a(n) = Sum_{k=0..n} k! * k^(n-k).
 * https://oeis.org/A367011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} k! * k^(n-k) (OEIS A367011)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t factorial = 1;  /* 0! */

    /* The k = 0 term is 0! * 0^(n) which equals 1 only for n = 0. */
    if (n == 0)
        sum = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        factorial = factorial * (seq_t)k;                /* k!       */
        seq_t term = factorial *
                     big_pow((seq_t)k, (seq_t)(n - k));    /* k^(n-k)  */
        sum += term;
    }
    return sum;
}
