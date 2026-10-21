/*
 * A252709  Sum_{k=0..n} k^(n+k)*(n-k)^k.
 * https://oeis.org/A252709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k^(n+k)*(n-k)^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t p1 = big_pow((seq_t)k,     (seq_t)(n + k));
        seq_t p2 = big_pow((seq_t)(n - k), (seq_t)k);
        sum += p1 * p2;
    }
    return sum;
}
