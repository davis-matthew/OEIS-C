/*
 * A300361  a(n) = 2^A000325(n).
 * https://oeis.org/A300361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^A000325(n) = 2^(2^n - n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"     /* seq_t pow2(idx_t exp); */
#include "../utils/big_pow.h" /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow2n = pow2(n);
    /* exponent = 2^n - n */
    seq_t exp = pow2n;
    exp -= (seq_t)n;
    /* a(n) = 2^exp */
    return big_pow((seq_t)2, exp);
}
