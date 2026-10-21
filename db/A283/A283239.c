/*
 * A283239  Number of ways to write n as x^2 + y^2 + z*(3*z-1)/2 with x,y,z
 *          integers such that x + 2*y is a square.
 * https://oeis.org/A283239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer triples (x,y,z) with * x^2 + y^2 +
 *            z*(3*z-1)/2 = n * and x + 2*y is a perfect square
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the required square‑root utility */
#include "../utils/floor_sqrt.h"
#include <stdint.h>
#include <math.h>

/* Helper: generalized pentagonal number for any integer z */
static inline seq_t pentagonal(int64_t z)
{
    __int128 zz = z;
    __int128 prod = zz * ( (3*zz) - 1 );
    prod /= 2;
    return (seq_t)prod;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    /* upper bound for |z|: roughly sqrt(2*n/3) + small safety margin */
    double d = sqrt(2.0 * (double)n / 3.0);
    int64_t maxz = (int64_t)d + 2;

    seq_t count = 0;

    for (int64_t z = -maxz; z <= maxz; ++z) {
        seq_t g = pentagonal(z);
        if (g > n_seq) continue;

        seq_t r = n_seq - g;
        int64_t maxy = (int64_t)floor_sqrt(r);

        for (int64_t y = -maxy; y <= maxy; ++y) {
            seq_t y2 = (seq_t)y * (seq_t)y;
            seq_t t  = r - y2;
            int64_t s = (int64_t)floor_sqrt(t);
            if ((seq_t)s * (seq_t)s != t) continue;

            /* Case x = +s */
            int64_t sum1 = s + 2*y;
            if (sum1 >= 0) {
                seq_t sum1sq = floor_sqrt((seq_t)sum1);
                if (sum1sq * sum1sq == (seq_t)sum1) count++;
            }

            /* Case x = -s, skip if s==0 (already counted) */
            if (s > 0) {
                int64_t sum2 = -s + 2*y;
                if (sum2 >= 0) {
                    seq_t sum2sq = floor_sqrt((seq_t)sum2);
                    if (sum2sq * sum2sq == (seq_t)sum2) count++;
                }
            }
        }
    }

    return count;
}
