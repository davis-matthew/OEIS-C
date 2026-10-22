/*
 * A350088  Number of ways to write n as t^2 + w^4 + 2*x^4 + 4*y^4 + 8*z^4,
 *          where t is a positive integer, and w,x,y,z are nonnegative
 *          integers.
 * https://oeis.org/A350088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of representations of n as t^2 + w^4 + 2*x^4 + 4*y^4 + 8*z^4
   with t>0 and w,x,y,z≥0.  a(1) is the first term. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static inline seq_t pow4_u64(uint64_t k)
{
    seq_t y = (seq_t)k * k;
    return y * y;      /* k⁴ as seq_t */
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;            /* target value */
    seq_t count = 0;               /* result accumulator */

    for (idx_t w = 0; ; ++w) {
        seq_t w4 = pow4_u64(w);    /* w⁴ */
        if (w4 > N) break;        /* further w would exceed N */

        seq_t sumw = w4;           /* partial sum without x,y,z */

        for (idx_t x = 0; ; ++x) {
            seq_t x4 = pow4_u64(x);/* x⁴ */
            seq_t sumwx = sumw + (seq_t)2 * x4;   /* w⁴ + 2x⁴ */
            if (sumwx > N) break;

            for (idx_t y = 0; ; ++y) {
                seq_t y4 = pow4_u64(y);/* y⁴ */
                seq_t sumwxy = sumwx + (seq_t)4 * y4; /* w⁴+2x⁴+4y⁴ */
                if (sumwxy > N) break;

                for (idx_t z = 0; ; ++z) {
                    seq_t z4 = pow4_u64(z);            /* z⁴ */
                    seq_t weight = sumwxy + (seq_t)8 * z4;  /* w⁴+2x⁴+4y⁴+8z⁴ */
                    if (weight > N) break;

                    seq_t rem = N - weight;             /* t² */
                    if (rem == 0) continue;             /* t must be >0 */

                    seq_t t = floor_sqrt(rem);          /* integer sqrt */
                    if (t * t == rem)                  /* exact square? */
                        ++count;
                }
            }
        }
    }

    return count;
}
