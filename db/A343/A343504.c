/*
 * A343504  a(n) is the least common multiple of the nonzero digits in
 *          factorial base expansion of n.
 * https://oeis.org/A343504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = lcm of the nonzero digits in the factorial‑base expansion
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 by convention */

    seq_t lcm = 1;
    idx_t i = 1;                  /* digit position (corresponding to i!) */
    idx_t m = n;                  /* working copy of n */

    while (m > 0) {
        idx_t base = i + 1;                   /* next factorial base */
        idx_t digit = m % base;               /* current digit (0 … i) */
        m /= base;                            /* reduce number */

        if (digit != 0) {
            seq_t d = (seq_t)digit;          /* convert digit to seq_t */
            seq_t g = gcd(lcm, d);           /* lcm(a,b)=a/gcd(a,b)*b */
            lcm = lcm / g * d;
        }
        ++i;
    }

    return lcm;
}
