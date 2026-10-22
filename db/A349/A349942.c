/*
 * A349942  Number of ways to write n as a^4 + b^2 + (c^4 + d^2)/25 with
 *          a,b,c,d nonnegative integers.
 * https://oeis.org/A349942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative integer solutions of * a^4 + b^2 +
 *            (c^4 + d^2)/25 = n
 * Range:     u8 n <= 2, u16 n <= 590, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    /* compute limits for a (a^4 <= n) */
    seq_t max_a = floor_sqrt(floor_sqrt((seq_t)n));
    seq_t na = (seq_t)n;  /* cast once for reuse */

    for (idx_t a = 0; a <= max_a; ++a) {
        seq_t a4 = (seq_t)a * a * a * a;
        seq_t rem_ab = na - a4;           /* n - a^4 >= 0 by construction */

        /* limits for b (b^2 <= rem_ab) */
        seq_t max_b = floor_sqrt(rem_ab);
        for (idx_t b = 0; b <= max_b; ++b) {
            seq_t b2 = (seq_t)b * b;
            seq_t r = rem_ab - b2;        /* remaining after a^4 + b^2 */

            /* We need c^4 + d^2 = 25 * r */
            seq_t target = r * (seq_t)25;
            /* limits for c (c^4 <= target) */
            seq_t max_c = floor_sqrt(floor_sqrt(target));
            for (idx_t c = 0; c <= max_c; ++c) {
                seq_t c4 = (seq_t)c * c * c * c;
                seq_t s = target - c4;    /* s = d^2 */
                seq_t d = floor_sqrt(s);
                if (d * d == s)          /* s is a perfect square */
                    ++count;
            }
        }
    }
    return count;
}
