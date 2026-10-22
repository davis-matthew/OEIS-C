/*
 * A381588  If n = Product (p_j^k_j) then a(n) = Product (lcm(p_j, k_j)), with
 *          a(1) = 1.
 * https://oeis.org/A381588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Π_{p^k || n} lcm(p, k) (a(1)=1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n <= 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* factor out the power of two first for a little speed gain */
    if ((m & 1ULL) == 0) {
        int k = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++k;
        }
        seq_t p = (seq_t)2;
        seq_t e = (seq_t)k;
        seq_t g = gcd(p, e);
        seq_t lcm = (p / g) * e;
        result *= lcm;
    }

    /* trial division for odd factors */
    for (uint64_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {
            int k = 0;
            while (m % d == 0) {
                m /= d;
                ++k;
            }
            seq_t p = (seq_t)d;
            seq_t e = (seq_t)k;
            seq_t g = gcd(p, e);
            seq_t lcm = (p / g) * e;
            result *= lcm;
        }
    }

    /* if a prime factor remains */
    if (m > 1) {
        seq_t p = (seq_t)m;   /* remaining prime */
        seq_t e = (seq_t)1;   /* exponent is 1 */
        seq_t g = gcd(p, e);
        seq_t lcm = (p / g) * e;
        result *= lcm;
    }

    return result;
}
