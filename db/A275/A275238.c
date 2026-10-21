/*
 * A275238  a(n) = n*(10^floor(log_10(n)+1) + 1) + (-1)^n.
 * https://oeis.org/A275238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n*(10^floor(log_10(n)+1) + 1) + (-1)^n
 * Range:     u8 n <= 9, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute the number of decimal digits of n.
       For n = 0 we treat it as one digit. */
    idx_t digits = 0;
    idx_t tmp = n;
    if (tmp == 0)
        digits = 1;
    else {
        while (tmp > 0) {
            tmp /= 10;
            digits++;
        }
    }

    /* 10^digits as a seq_t. */
    seq_t pow10 = pow_uint(10, digits);

    /* Base result: n * (10^digits + 1). */
    seq_t res = (seq_t)n * (pow10 + 1);

    /* Add (-1)^n: +1 if n is even, -1 if n is odd. */
    if (n & 1)    /* odd n */
        res -= 1;
    else          /* even n */
        res += 1;

    return res;
}
