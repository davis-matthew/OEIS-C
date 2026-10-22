/*
 * A324268  a(n) = 31*11^(5*n).
 * https://oeis.org/A324268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*161051^n
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* 161051 = 11^5 */
    seq_t base = (seq_t)161051;
    seq_t exp  = (seq_t)n;
    seq_t pow  = big_pow(base, exp);
    return seq_mul(pow, (seq_t)31);
}
