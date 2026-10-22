/*
 * A360172  Sequences of length n in [n] not starting with their minimum
 *          value.
 * https://oeis.org/A360172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^n - sum_{k=1}^n k^(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offsets start at 1, but guard against 0 */
    seq_t total = big_pow((seq_t)n, (seq_t)n);   /* n^n */
    seq_t sum   = 0;
    idx_t exp   = n - 1;
    for (idx_t k = 1; k <= n; ++k)
        sum += big_pow((seq_t)k, (seq_t)exp);   /* k^(n-1) */
    return total - sum;
}
