/*
 * A275656  Number of ordered ways to write n as 4^k*(1+x^2+y^2)+z^2, where
 *          k,x,y,z are nonnegative integers with x <= y <= z.
 * https://oeis.org/A275656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered quadruples (k,x,y,z) of nonnegative
 *            integers satisfying n = 4^k * (1 + x^2 + y^2) + z^2 with the
 *            additional restriction x <= y <= z. The function computes this
 *            count for any n >= 1
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
    if (n == 0) return 0;          /* only used for internal safety */
    seq_t N = (seq_t)n;
    seq_t count = 0;

    /* iterate over possible exponents k for 4^k */
    seq_t pow4 = 1;                /* current value of 4^k */
    for (idx_t k = 0; ; ++k, pow4 <<= 2)   /* pow4 *= 4 each step */
    {
        if (pow4 > N) break;       /* further powers exceed N, no more k’s */

        seq_t M = N / pow4;        /* maximum value of (1 + x^2 + y^2) allowed */

        /* x ranges from 0 up to sqrt(M-1) because 1 + x^2 <= M */
        for (idx_t x = 0; ; ++x)
        {
            seq_t xsq = (seq_t)x * (seq_t)x;
            if (xsq > M - 1) break;    /* 1 + x^2 would exceed M */

            /* y ranges from x up to sqrt(M-1 - x^2) */
            seq_t y_limit_sq = M - 1 - xsq;
            seq_t y_limit = floor_sqrt(y_limit_sq);

            for (idx_t y = x; y <= y_limit; ++y)
            {
                seq_t ysq = (seq_t)y * (seq_t)y;
                seq_t sumXY = 1 + xsq + ysq;          /* 1 + x^2 + y^2 */

                /* 4^k * sumXY must not exceed N */
                seq_t term = pow4 * sumXY;
                if (term > N) continue;               /* skip if over N */

                seq_t rem = N - term;                /* z^2 */
                seq_t z = floor_sqrt(rem);
                if (z * z == rem && (seq_t)z >= (seq_t)y)
                    ++count;
            }
        }
    }

    return count;
}
