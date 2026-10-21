/*
 * A256162  Positive integers a(n) such that number of digits in decimal
 *          expansion of a(n)^a(n) is divisible by a(n).
 * https://oeis.org/A256162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10^floor(n/2) - 2 if n is even * a(n) = 10^floor(n/2) - 1
 *            if n is odd > 1 * a(1) = 1 * This follows from the OEIS
 *            definition A256162
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Off‐by-one: we assume the offset is 1 (i.e. a(1) = 1). */
    if (n == (idx_t)1)               /* a(1) = 1 */
        return (seq_t)1;

    idx_t k = n / (idx_t)2;          /* floor(n/2) */
    seq_t p = pow10(k);              /* 10^k */

    if (n & (idx_t)1)                /* odd n > 1  -> 10^k - 1 */
        return p - (seq_t)1;
    else                             /* even n     -> 10^k - 2 */
        return p - (seq_t)2;
}
