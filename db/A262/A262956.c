/*
 * A262956  Number of ordered pairs (x,y) with x >= 0 and y > 0 such that n -
 *          x^4 - y*(y+1)/2 is a square or a square minus 1.
 * https://oeis.org/A262956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (x,y) with x>=0, y>0 such that n
 *            - x^4 - y*(y+1)/2 is a perfect square or one less than a perfect
 *            square
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t cnt = 0;

    /* loop over x where x^4 <= N */
    for (idx_t x = 0; ; ++x) {
        seq_t x4 = (seq_t)x * x * x * x;
        if (x4 > N) break;

        seq_t remainder_limit = N - x4;

        /* loop over y where y*(y+1)/2 <= remainder_limit */
        for (idx_t y = 1; ; ++y) {
            seq_t tri = (seq_t)y * ((seq_t)y + 1) / 2;
            if (tri > remainder_limit) break;

            seq_t rem = remainder_limit - tri;

            /* Check if rem is a square or one less than a square */
            if (rem == 0) {
                cnt++;          /* 0 is a perfect square */
                continue;
            }
            seq_t s = floor_sqrt(rem);
            if (s * s == rem) {
                cnt++;
            } else {
                seq_t rp1 = rem + 1;
                seq_t rps = floor_sqrt(rp1);
                if (rps * rps == rp1) {
                    cnt++;
                }
            }
        }
    }

    return cnt;
}
