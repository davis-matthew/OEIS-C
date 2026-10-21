/*
 * A275002  a(n) = 2*n^(2*n^2 + 2).
 * https://oeis.org/A275002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^(2*n^2 + 2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)(2ULL * n * n + 2);
    seq_t val  = big_pow(base, exp);
    return (seq_t)(2 * val);
}
