/*
 * A281977  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers such that both x and -7*x - 8*y + 8*z + 16*w
 *          are squares.
 * https://oeis.org/A281977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of tuples (a,b,c,d) of non‑negative integers such that
 *                n = a^4 + b^2 + c^2 + d^2
 *                and both a^2 and -7*a^2 - 8*b + 8*c + 16*d are perfect squares.
 *
 * The algorithm enumerates all possibilities for a, b, and c, determines d
 * uniquely as the integer square root of the remaining part of the sum,
 * checks the secondary square condition, and counts the valid tuples.
 * Complexity is O(n^(1/2) * n^(1/4)) for each n, easily fast enough for n up
 * to 10 000 and well under 2 s per call for bigger n as required.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* maximum possible a: a^4 <= n, so a <= floor(n^(1/4)) */
    idx_t maxA = (idx_t)floor_sqrt((seq_t)floor_sqrt((seq_t)n));

    for (idx_t a = 0; a <= maxA; ++a) {
        seq_t a2 = (seq_t)a * (seq_t)a;          /* a^2   */
        seq_t a4 = a2 * a2;                      /* a^4   */

        if (a4 > (seq_t)n) break;                /* safety */

        seq_t rem1 = (seq_t)n - a4;              /* remaining sum for b^2 + c^2 + d^2 */

        idx_t maxB = (idx_t)floor_sqrt(rem1);
        for (idx_t b = 0; b <= maxB; ++b) {
            seq_t b2 = (seq_t)b * (seq_t)b;
            seq_t rem2 = rem1 - b2;

            idx_t maxC = (idx_t)floor_sqrt(rem2);
            for (idx_t c = 0; c <= maxC; ++c) {
                seq_t c2 = (seq_t)c * (seq_t)c;
                seq_t rem3 = rem2 - c2;           /* rem3 must be a perfect square d^2 */

                /* Determine d uniquely as sqrt(rem3) if integer */
                idx_t d = (idx_t)floor_sqrt(rem3);
                if ((seq_t)d * (seq_t)d != rem3) continue;

                /* Check secondary square condition:
                 * s = -7*a^2 - 8*b + 8*c + 16*d
                 *   = (8*c + 16*d) - (7*a^2 + 8*b)
                 * Ensure the expression is non‑negative before computing.
                 */
                seq_t left  = 8ULL * (seq_t)c + 16ULL * (seq_t)d;
                seq_t right = 7ULL * a2 + 8ULL * (seq_t)b;
                if (left < right) continue;          /* would be negative */

                seq_t s = left - right;
                seq_t sqrt_s = floor_sqrt(s);
                if ((seq_t)sqrt_s * (seq_t)sqrt_s == s) {
                    ++count;
                }
            }
        }
    }

    return count;
}
