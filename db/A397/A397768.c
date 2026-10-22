/*
 * A397768  a(n) = sum of powers of 2 where the exponents are the digits of n.
 * https://oeis.org/A397768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = Σ_{digits d in n} 2^d
 * Range:     u8 n <= 7, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Special case: a(0) = 2^0 = 1 */
    if (n == 0) {
        return pow2(0);
    }

    seq_t sum = 0;
    while (n > 0) {
        uint8_t d = n % 10;          /* least‑significant decimal digit */
        sum += pow2((idx_t)d);       /* add 2^d to the running total */
        n /= 10;                     /* shift right one decimal digit */
    }
    return sum;
}
