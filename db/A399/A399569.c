/*
 * A399569  a(n) = Product_{k=1..n} (2*k-1)^(2*k-1).
 * https://oeis.org/A399569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (2*k-1)^(2*k-1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
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
        seq_t base = (seq_t)(2 * k - 1);
        seq_t exp  = base;
        seq_t term = big_pow(base, exp);
        result = result * term;
    }
    return result;
}
