/*
 * A301805  Number of ways to write 3*n^2 as x^2 + 10*y^2 + 2^z, where x, y
 *          and z are nonnegative integers with z > 3.
 * https://oeis.org/A301805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 698, u64 n <= 800,
 *            u128 n <= 800, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A301805   Number of representations of 3*n^2
 *  as x^2 + 10*y^2 + 2^z with x,y,z ≥ 0 and z > 3.
 *
 *  The function iterates over all admissible exponents z (z ≥ 4),
 *  then over all y such that 10*y^2 ≤ 3*n^2 – 2^z,
 *  and checks whether the remaining value is a perfect square.
 *  The count of solutions is returned as seq_t.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* 3*n^2 using seq_t arithmetic to avoid overflow             */
    seq_t M = (seq_t)3 * n * n;

    seq_t count = 0;                     /* final result */

    /* start with z = 4  → 2^4 = 16                           */
    seq_t pow2 = ((seq_t)1) << 4;        /* 2^4 */

    for (int z = 4; ; ++z) {
        if (pow2 > M) break;             /* no larger 2^z can fit */

        /* remaining part after subtracting 2^z                 */
        seq_t base = M - pow2;

        /* loop over y while 10*y^2 ≤ base                       */
        for (uint64_t y = 0; ; ++y) {
            seq_t y2 = (seq_t)y * (seq_t)y;
            seq_t y10 = (seq_t)10 * y2;
            if (y10 > base) break;                 /* y too big */

            seq_t rem = base - y10;                /* candidate square */

            seq_t x = isqrt_seq(rem);             /* floor sqrt of rem   */
            if (x * x == rem)                     /* perfect square? */
                count += 1;
        }

        pow2 <<= 1;      /* next power of two: 2^(z+1) */
    }

    return count;
}
