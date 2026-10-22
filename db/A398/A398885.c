/*
 * A398885  Antidiagonal sums of the array A398884.
 * https://oeis.org/A398885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (1 + k^(n-k)*(n - k))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t pow = big_pow((seq_t)k, (seq_t)(n - k));
        sum += pow * (seq_t)(n - k) + (seq_t)1;
    }
    return sum;
}
