/*
 * A395435  a(n) = sigma(n) - usigma(n) + tau(rad(n)).
 * https://oeis.org/A395435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d^(1 - [gcd(d,n/d) = 1])
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: for each divisor d of n, add 1 if gcd(d,n/d)=1, else add d. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            seq_t g = gcd((seq_t)d1, (seq_t)d2);
            if (g == (seq_t)1)
                sum += (seq_t)1;
            else
                sum += (seq_t)d1;

            if (d1 != d2) {
                seq_t g2 = gcd((seq_t)d2, (seq_t)d1);
                if (g2 == (seq_t)1)
                    sum += (seq_t)1;
                else
                    sum += (seq_t)d2;
            }
        }
    }
    return sum;
}
