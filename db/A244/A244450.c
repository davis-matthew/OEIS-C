/*
 * A244450  Powers of ten billion.
 * https://oeis.org/A244450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10000000000^n
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 3,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10000000000 == 10^10, so a(n) = (10^10)^n = 10^(10*n) */
    idx_t exp = n * 10;          /* exponent = 10 * n */
    return pow10(exp);           /* pow10(e) returns 10^e */
}
