/*
 * A270921  Number of ordered ways to write n as x*(3x+2) + y*(5y+1)/2 - z^4,
 *          where x and y are integers, and z is a nonnegative integer with
 *          z^4 <= n.
 * https://oeis.org/A270921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ (x,y,z) : x,y∈ℤ , z∈ℕ , z^4≤n ,
 *            x*(3x+2)+y*(5y+1)/2‑z^4 = n }|
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm enumerates all admissible z, computes the remaining
   target value, then loops over all feasible x and y by using integer
   bounds derived from the quadratic inequalities. */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* loop over all z with z^4 <= n */
    for (idx_t z = 0;; ++z)
    {
        seq_t z2 = (seq_t)z * (seq_t)z;
        seq_t z4 = z2 * z2;
        if (z4 > n)
            break;                   /* larger z will make z^4 > n */
        seq_t target = (seq_t)n + z4;/* value that must be expressed by x and y */

        /* bound for |x| : 3x^2+2x <= target  =>  |x| <= sqrt(target/3)+1  */
        seq_t tmp = target / 3;                 /* floor( target/3 ) */
        seq_t sqrt_tmp = floor_sqrt(tmp);
        idx_t xmax = (idx_t)sqrt_tmp + 1;        /* cast is safe for small n */

        /* enumerate all integer x in [-xmax,xmax] */
        for (long long x = -(long long)xmax; x <= (long long)xmax; ++x)
        {
            long long lx = x;
            seq_t ex = (seq_t)(3 * lx * lx + 2 * lx);
            if (ex > target)
                continue;
            seq_t rem = target - ex;            /* remaining value for y part */

            /* bound for |y| : y*(5y+1)/2 <= rem  =>  |y| <= sqrt(2rem/5)+1 */
            seq_t t2   = rem * 2;                /* 2*rem */
            seq_t tmp2 = t2 / 5;                /* floor division */
            seq_t sqrt_tmp2 = floor_sqrt(tmp2);
            idx_t ymax = (idx_t)sqrt_tmp2 + 1;

            for (long long y = -(long long)ymax; y <= (long long)ymax; ++y)
            {
                long long ly = y;
                seq_t fy = (seq_t)(5 * ly * ly + ly);   /* 5y^2 + y */
                fy /= 2;                                 /* exact division */
                if (fy == rem)
                    ++count;
            }
        }
    }
    return count;
}
