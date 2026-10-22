/*
 * A361476  Antidiagonal sums of A361475.
 * https://oeis.org/A361476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n+2} (k^(n-k+2)-1)/(k-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 27, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 2, limit = n + 2; k <= limit; ++k)
    {
        idx_t exp = n - k + 2;                      /* n-k+2 ≥ 0 */
        seq_t pow_val = big_pow((seq_t)k, (seq_t)exp); /* k^(n-k+2) */
        seq_t denom = (seq_t)(k - 1);                /* k-1 */
        seq_t term = (pow_val - 1) / denom;          /* integer division */
        sum += term;
    }
    return sum;
}
