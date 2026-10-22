/*
 * A306249  Number of ways to write n as x*(2x-1) + y*(3y-1) + z*(4z-1) +
 *          w*(5w-1), where x,y,z are nonnegative integers and w is 0 or 1.
 * https://oeis.org/A306249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n =
 *            x*(2x-1)+y*(3y-1)+z*(4z-1)+w*(5w-1), with x,y,z ≥ 0 and w ∈
 *            {0,1}
 * Range:     u8 n <= 759, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n_seq = (seq_t)n;

    for (idx_t x = 0; ; ++x) {
        seq_t valx = (seq_t)x * ((seq_t)2 * (seq_t)x - (seq_t)1);
        if (valx > n_seq) break;
        for (idx_t y = 0; ; ++y) {
            seq_t valy = (seq_t)y * ((seq_t)3 * (seq_t)y - (seq_t)1);
            seq_t sumxy = valx + valy;
            if (sumxy > n_seq) break;
            for (idx_t z = 0; ; ++z) {
                seq_t valz = (seq_t)z * ((seq_t)4 * (seq_t)z - (seq_t)1);
                seq_t sumxyz = sumxy + valz;
                if (sumxyz > n_seq) break;

                /* w = 0 */
                if (sumxyz == n_seq) ++count;
                /* w = 1 */
                if (n_seq >= 4 && sumxyz == n_seq - (seq_t)4) ++count;
            }
        }
    }

    return count;
}
