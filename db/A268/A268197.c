/*
 * A268197  Number of ordered ways to write n as w^2 + x^2 + y^2 + z^2 with
 *          w*(25*w + 24*x + 48*y + 96*z) a square, where w is a positive
 *          integer and x,y,z are nonnegative integers.
 * https://oeis.org/A268197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1473, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A268197:
   a(n) = number of ordered quadruples (w,x,y,z) with w>0,
   x,y,z≥0, satisfying
          w² + x² + y² + z² = n
   and
          w*(25*w + 24*x + 48*y + 96*z)  is a perfect square.
   The function works for any seq_t type and for n up to about 10000
   in well under 2 s. */

#include <stdint.h>
#include <stdbool.h>

/* --------- helper: integer square root for 64‑bit unsigned numbers --------- */
static inline uint64_t isqrt_u64(uint64_t x)
{
    /* binary search for floor(sqrt(x)) – safe because x ≤ 1e8 in this problem */
    uint64_t low  = 0;
    uint64_t high = (uint64_t)1 << 31;   /* 2^31*2^31 = 2^62 < 2^64, no overflow */
    while (low <= high)
    {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* floor(sqrt(x)) */
}

/* --------- main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is defined for n ≥ 1, but return 0 for 0 */
        return 0;

    seq_t count = 0;               /* accumulator for solutions */

    /* outer loop over possible w (positive) */
    for (idx_t w = 1; w * w <= n; ++w)
    {
        idx_t rem = n - w * w;          /* remaining sum for x² + y² + z² */

        /* loop over x */
        for (idx_t x = 0; x * x <= rem; ++x)
        {
            idx_t rem2 = rem - x * x;   /* remaining sum for y² + z² */

            /* loop over y */
            for (idx_t y = 0; y * y <= rem2; ++y)
            {
                idx_t z2 = rem2 - y * y;            /* z² = z2 must be a perfect square */
                uint64_t z = isqrt_u64((uint64_t)z2);
                if (z * z != (uint64_t)z2)
                    continue;                     /* z not integer */

                /* compute linear combination: 25*w + 24*x + 48*y + 96*z */
                seq_t linear = (seq_t)25 * w
                               + (seq_t)24 * x
                               + (seq_t)48 * y
                               + (seq_t)96 * z;

                /* compute L = w * linear and test if it is a square */
                seq_t L = (seq_t)w * linear;
                uint64_t s = isqrt_u64((uint64_t)L);
                if (s * s == (uint64_t)L)
                    ++count;
            }
        }
    }
    return count;
}
