/*
 * A357754  a(n) is the largest square with n binary digits.
 * https://oeis.org/A357754
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(sqrt(2^n - 1))^2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   floor_sqrt, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t val = seq_pow((seq_t)2, n);
    val -= (seq_t)1;
    seq_t s = floor_sqrt(val);
    return s * s;
}
