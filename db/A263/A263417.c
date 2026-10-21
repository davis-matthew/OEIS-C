/*
 * A263417  a(n) = Product_{k=0..n} (3*k+2)^(n-k).
 * https://oeis.org/A263417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0..n} (3*k+2)^(n-k)
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
    for (idx_t k = 0; k <= n; ++k) {
        seq_t base     = (seq_t)(3 * k + 2);
        seq_t exponent = (seq_t)(n - k);
        seq_t term     = big_pow(base, exponent);
        result *= term;
    }
    return result;
}
