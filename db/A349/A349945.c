/*
 * A349945  Number of ways to write n as a^4 + b^2 + (c^4 + d^2)/5 with
 *          a,b,c,d nonnegative integers.
 * https://oeis.org/A349945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions (a,b,c,d) to a^4
 *            + b^2 + (c^4 + d^2)/5 = n
 * Range:     u8 n <= 12, u16 n <= 2278, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t A = 0;; ++A) {
        seq_t A4 = (seq_t)A * A * A * A;
        if (A4 > n) break;

        for (idx_t B = 0;; ++B) {
            seq_t B2 = (seq_t)B * B;
            if (A4 + B2 > n) break;

            seq_t remaining = n - (A4 + B2);          /* remaining to be filled by (c^4 + d^2)/5 */
            seq_t target   = remaining * ((seq_t)5);   /* c^4 + d^2 must equal this value */

            for (idx_t C = 0;; ++C) {
                seq_t C4 = (seq_t)C * C * C * C;
                if (C4 > target) break;

                seq_t remainder = target - C4;             /* d^2 must equal this */
                seq_t d = floor_sqrt(remainder);           /* integer square root */
                if (d * d == remainder) ++count;          /* perfect square ⇒ valid d */
            }
        }
    }

    return count;
}
