/*
 * A320140  Number of integer solutions to a^2 + 2*b^2 + 3*c^2 + 5*d^2 = n.
 * https://oeis.org/A320140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of integer solutions to a^2 + 2*b^2 + 3*c^2 + 5*d^2 = n
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* limit_a is the largest a such that a^2 <= n */
    idx_t limit_a = (idx_t)floor_sqrt((seq_t)n);
    seq_t count = 0;

    for (idx_t a = 0; a <= limit_a; ++a)
    {
        seq_t a2 = (seq_t)a * a;
        seq_t sign_a = (a == 0) ? 1 : 2;
        seq_t rem1 = (seq_t)n - a2;                  /* n - a^2 */

        /* limit_b is the largest b such that 2*b^2 <= rem1 */
        idx_t limit_b = (idx_t)floor_sqrt(rem1 / 2);
        for (idx_t b = 0; b <= limit_b; ++b)
        {
            seq_t b2 = (seq_t)b * b;
            seq_t sign_b = (b == 0) ? 1 : 2;
            seq_t rem2 = rem1 - 2 * b2;              /* n - a^2 - 2*b^2 */

            /* limit_c is the largest c such that 3*c^2 <= rem2 */
            idx_t limit_c = (idx_t)floor_sqrt(rem2 / 3);
            for (idx_t c = 0; c <= limit_c; ++c)
            {
                seq_t c2 = (seq_t)c * c;
                seq_t sign_c = (c == 0) ? 1 : 2;
                seq_t rem3 = rem2 - 3 * c2;          /* n - a^2 - 2*b^2 - 3*c^2 */

                if (rem3 < 0) break;                /* further c only larger */

                if (rem3 % 5 != 0) continue;        /* d^2 must be integer */

                seq_t d2 = rem3 / 5;
                seq_t d = floor_sqrt(d2);
                if (d * d != d2) continue;          /* not a perfect square */

                seq_t sign_d = (d == 0) ? 1 : 2;
                count += sign_a * sign_b * sign_c * sign_d;
            }
        }
    }

    return count;
}
