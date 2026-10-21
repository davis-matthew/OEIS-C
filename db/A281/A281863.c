/*
 * A281863  Alternating powers of 60 and 10 times powers of 60.
 * https://oeis.org/A281863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 60^n, a(2*n+1) = 10*60^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    idx_t r = n / 2;
    seq_t base = (seq_t)60;
    seq_t val = big_pow(base, (seq_t)r);
    if (n % 2 == 1)
        val = mul_small(val, 10);
    return val;
}
