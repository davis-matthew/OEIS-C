/*
 * A349901  a(n) = Sum_{k=0..n} k^(3*n).
 * https://oeis.org/A349901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^(3*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t exp = (seq_t)n * 3;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t base = (seq_t)k;
        seq_t term = big_pow(base, exp);
        sum += term;
    }
    return sum;
}
