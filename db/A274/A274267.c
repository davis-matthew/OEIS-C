/*
 * A274267  a(n) = (4*n - 1)^(n-1).
 * https://oeis.org/A274267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n - 1)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)4 * (seq_t)n - (seq_t)1;
    seq_t exp  = (seq_t)(n - (idx_t)1);
    return big_pow(base, exp);
}
