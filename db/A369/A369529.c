/*
 * A369529  a(n) = gcd(A007953(n), A007954(n)).
 * https://oeis.org/A369529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = gcd(product of decimal digits of n, sum of decimal digits
 *            of n)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t sum  = 0;
    idx_t t = n;

    if (t == 0) {
        prod = 0;
        sum  = 0;
    } else {
        while (t > 0) {
            idx_t d = t % 10;
            sum += d;
            if (d == 0) {
                prod = 0;
            } else if (prod != 0) {
                prod *= (seq_t)d;
            }
            t /= 10;
        }
    }
    return gcd(prod, sum);
}
