/*
 * A366305  a(n) = Product_{k=1..n} (k^n + (k-1)^n).
 * https://oeis.org/A366305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (k^n + (k-1)^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t pow_k  = big_pow((seq_t)k,       (seq_t)n);
        seq_t pow_k1 = big_pow((seq_t)(k-1),   (seq_t)n);
        result = result * (pow_k + pow_k1);
    }
    return result;
}
