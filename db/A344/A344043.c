/*
 * A344043  a(n) = n * Sum_{d|n} sigma(d)^3 / d.
 * https://oeis.org/A344043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * Sum_{d|n} sigma(d)^3 / d
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 479, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        idx_t e = n / d; /* complementary divisor */

        /* contribution of divisor d */
        seq_t s_d   = sigma_of(d);
        seq_t s_d2  = s_d * s_d;
        seq_t s_d3  = s_d2 * s_d;
        result += ((seq_t)(n / d)) * s_d3;

        if (d != e) { /* contribution of divisor e */
            seq_t s_e   = sigma_of(e);
            seq_t s_e2  = s_e * s_e;
            seq_t s_e3  = s_e2 * s_e;
            result += ((seq_t)(n / e)) * s_e3;
        }
    }

    return result;
}
