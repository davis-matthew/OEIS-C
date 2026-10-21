/*
 * A270417  Number of integer-sided right triangles with semiperimeter n.
 * https://oeis.org/A270417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer right triangles with semiperimeter n.
 *            Equivalently, the number of solutions of x*y*(y+z) = n with y >
 *            z ≥ 1, gcd(y,z)=1, and y and z of opposite parity
 * Range:     u8 n <= 8703, u16 n <= 8703, u32 n <= 8703, u64 n <= 8703,
 *            u128 n <= 8703, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* iterate over all divisors y of n */
    for (idx_t y = 1; y <= n; ++y) {
        if (n % y != 0)
            continue;

        idx_t t = n / y;  /* t = d * (y + z) for some d */

        /* iterate over all divisors d of t */
        for (idx_t d = 1; d * d <= t; ++d) {
            if (t % d != 0)
                continue;

            /* first divisor d */
            {
                idx_t sum = t / d;            /* sum = y + z */
                idx_t z = sum > y ? sum - y : 0;  /* z > 0 automatically checked */
                if (z > 0 && z < y) {
                    if ((y & 1U) != (z & 1U) /* opposite parity */) {
                        seq_t g = gcd((seq_t)y, (seq_t)z);
                        if (g == (seq_t)1)
                            ++count;
                    }
                }
            }

            /* second divisor d2 = t / d, if distinct */
            if (d * d != t) {
                idx_t d2 = t / d;
                idx_t sum = t / d2;            /* equals (y + z) again */
                idx_t z = sum > y ? sum - y : 0;
                if (z > 0 && z < y) {
                    if ((y & 1U) != (z & 1U)) {
                        seq_t g = gcd((seq_t)y, (seq_t)z);
                        if (g == (seq_t)1)
                            ++count;
                    }
                }
            }
        }
    }

    return count;
}
