/*
 * A320189  Number of integer solutions to a^2 + 2*b^2 + 3*c^2 + 8*d^2 = n.
 * https://oeis.org/A320189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions of a^2 + 2*b^2 + 3*c^2 +
 *            8*d^2 = n
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t ans = 0;

    for (idx_t d = 0; ; ++d) {
        seq_t d2   = (seq_t)d * (seq_t)d;
        seq_t dterm = 8 * d2;
        if (dterm > n) break;
        seq_t w_d = (d == 0 ? (seq_t)1 : (seq_t)2);

        for (idx_t c = 0; ; ++c) {
            seq_t c2   = (seq_t)c * (seq_t)c;
            seq_t cdterm = dterm + 3 * c2;
            if (cdterm > n) break;
            seq_t w_c = (c == 0 ? (seq_t)1 : (seq_t)2);

            for (idx_t b = 0; ; ++b) {
                seq_t b2      = (seq_t)b * (seq_t)b;
                seq_t bcterm  = cdterm + 2 * b2;
                if (bcterm > n) break;
                seq_t w_b = (b == 0 ? (seq_t)1 : (seq_t)2);

                seq_t rem = (seq_t)(n - bcterm);
                seq_t s   = floor_sqrt(rem);
                if (s * s == rem) {
                    seq_t w_a = (rem == 0 ? (seq_t)1 : (seq_t)2);
                    seq_t w   = w_d;
                    w *= w_c;
                    w *= w_b;
                    w *= w_a;
                    ans += w;
                }
            }
        }
    }

    return ans;
}
