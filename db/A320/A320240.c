/*
 * A320240  Expansion of theta_3(q) * theta_3(q^3) * theta_3(q^5) *
 *          theta_3(q^7), where theta_3() is the Jacobi theta function.
 * https://oeis.org/A320240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (a1,a2,a3,a4) of * a1^2 +
 *            3·a2^2 + 5·a3^2 + 7·a4^2 = n * The implementation enumerates all
 *            non‑negative solutions, * then counts the sign variations for
 *            each variable
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t ans = 0;
    seq_t n_seq = n;                               // convert index to seq_t

    idx_t max4 = (idx_t)floor_sqrt(n_seq / 7);     // a4 ≤ sqrt(n/7)
    for (idx_t a4 = 0; a4 <= max4; ++a4)
    {
        seq_t t4 = ((seq_t)7) * (seq_t)a4 * (seq_t)a4;
        seq_t rem1 = n_seq - t4;                   // remaining after a4
        idx_t max3 = (idx_t)floor_sqrt(rem1 / 5);  // a3 ≤ sqrt(rem1/5)
        for (idx_t a3 = 0; a3 <= max3; ++a3)
        {
            seq_t t3 = ((seq_t)5) * (seq_t)a3 * (seq_t)a3;
            seq_t rem2 = rem1 - t3;                // remaining after a3
            idx_t max2 = (idx_t)floor_sqrt(rem2 / 3); // a2 ≤ sqrt(rem2/3)
            for (idx_t a2 = 0; a2 <= max2; ++a2)
            {
                seq_t t2 = ((seq_t)3) * (seq_t)a2 * (seq_t)a2;
                seq_t rem3 = rem2 - t2;            // remaining for a1^2

                seq_t sq = floor_sqrt(rem3);
                if (sq * sq == rem3)              // perfect square ? a1 exists
                {
                    seq_t count = 1;
                    if (a4) count *= 2;           // sign choices for a4
                    if (a3) count *= 2;           // sign choices for a3
                    if (a2) count *= 2;           // sign choices for a2
                    if (sq) count *= 2;           // sign choices for a1
                    ans += count;
                }
            }
        }
    }
    return ans;
}
