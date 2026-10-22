/*
 * A356672  a(n) = n! * Sum_{k=0..n} k^(2*(n-k))/k!.
 * https://oeis.org/A356672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} k^(2*(n-k))/k!
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t ratio = 1;          /* ratio = n! / k! for current k */

    for (idx_t i = 0; i <= n; ++i)
    {
        idx_t k = n - i;

        if (i > 0)
            ratio = ratio * (seq_t)(k + 1);   /* update n! / k! */

        seq_t pow  = big_pow((seq_t)k, (seq_t)(2 * (n - k)));
        seq_t term = ratio * pow;

        sum += term;
    }

    return sum;
}
