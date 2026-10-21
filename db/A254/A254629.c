/*
 * A254629  Number of ways to write n as x^2 + y*(y+1) + z*(4*z+1) with x,y,z
 *          nonnegative integers.
 * https://oeis.org/A254629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of ways to write n as x^2 + y*(y+1) + z*(4*z+1) with
 *            x,y,z ≥ 0
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t N = (seq_t)n;

    /* iterate over z */
    for (idx_t z = 0; ; ++z) {
        seq_t zseq = (seq_t)z;
        seq_t fz = zseq * ((seq_t)4 * zseq + 1);
        if (fz > N) break;

        seq_t m = N - fz;

        /* iterate over y */
        for (idx_t y = 0; ; ++y) {
            seq_t yseq = (seq_t)y;
            seq_t fy = yseq * (yseq + 1);
            if (fy > m) break;

            seq_t rem = m - fy;
            seq_t sq = floor_sqrt(rem);
            if (sq * sq == rem) {
                ++count;
            }
        }
    }

    return (seq_t)count;
}
