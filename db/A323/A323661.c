/*
 * A323661  a(n) = Product_{k=0..n} (k^12 + (n-k)^12).
 * https://oeis.org/A323661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^12 + (n-k)^12)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* a(0) = 0 by definition */
    seq_t prod = 1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t k_pow = big_pow((seq_t)k, 12);
        seq_t nk_pow = big_pow((seq_t)(n - k), 12);
        seq_t term = k_pow + nk_pow;
        prod *= term;
    }
    return prod;
}
