/*
 * A271410  LCM of exponents in binary expansion of 2n.
 * https://oeis.org/A271410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = LCM{ i+1 | the i-th bit of n is 1 } , with a(0)=1
 * Range:     u8 n <= 91, u16 n <= 5331, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t lcm = (seq_t)1;
    idx_t pos = 0;

    while (n) {
        if (n & 1) {
            seq_t exp = (seq_t)(pos + 1);
            seq_t g = gcd(lcm, exp);
            lcm = (lcm / g) * exp;
        }
        n >>= 1;
        pos++;
    }

    return lcm;
}
