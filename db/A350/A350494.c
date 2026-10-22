/*
 * A350494  Divide n by the greatest common divisor of the nonzero digits of
 *          n.
 * https://oeis.org/A350494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = n / gcd(non‑zero decimal digits of n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t g = 0;                 /* GCD of non‑zero digits */
    idx_t t = n;                 /* temporary copy for digit extraction */

    while (t > 0) {
        int d = t % 10;          /* current decimal digit */
        t /= 10;                 /* advance to next digit */
        if (d != 0) {
            seq_t sd = (seq_t)d;
            if (g == 0)
                g = sd;          /* first non‑zero digit */
            else
                g = gcd(g, sd);  /* update GCD */
        }
    }

    seq_t m = (seq_t)n;          /* convert index to seq_t */
    return m / g;                /* a(n) */
}
