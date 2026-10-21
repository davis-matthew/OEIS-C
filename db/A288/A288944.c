/*
 * A288944  Number of automorphisms in the n-halved cube graph.
 * https://oeis.org/A288944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(n-1) · n! for all n≥3, except a(2)=2 and a(4)=2^n · n!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t pow;
    if (n == 4)
        pow = big_pow((seq_t)2, (seq_t)n);          /* 2^n for n = 4 */
    else
        pow = big_pow((seq_t)2, (seq_t)(n - 1));     /* 2^(n-1) for others */

    return fact * pow;
}
