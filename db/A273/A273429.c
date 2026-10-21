/*
 * A273429  Number of ordered ways to write n as x^6 + y^2 + z^2 + w^2, where
 *          x,y,z,w are nonnegative integers with y <= z <= w.
 * https://oeis.org/A273429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered tuples (x,y,z,w) of non‑negative
 *            integers * such that n = x^6 + y^2 + z^2 + w^2 and y ≤ z ≤ w.
 *            This implementation * counts the solutions by brute force, which
 *            is fast enough for n up to about * 10 000 (≈50 000 elementary
 *            operations per call)
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

    /* iterate over the sixth‑power part x^6, where x ≥ 0 and x^6 ≤ n */
    for (idx_t x = 0;; ++x)
    {
        /* compute x^6 as seq_t; x is very small (≤ 4 for n ≤ 10000) */
        seq_t xsq = (seq_t)x * (seq_t)x;        /* x² */
        seq_t x4  = xsq * xsq;                 /* x⁴ */
        seq_t x6  = x4 * xsq;                  /* x⁶ */
        if (x6 > (seq_t)n)
            break;

        seq_t t = (seq_t)n - x6;               /* remaining value to be expressed
                                                 as y² + z² + w² */

        /* iterate over y, with y² ≤ t */
        for (idx_t y = 0;; ++y)
        {
            seq_t y2 = (seq_t)y * (seq_t)y;
            if (y2 > t)
                break;

            seq_t t1 = t - y2;                 /* remaining value for z² + w² */

            /* iterate over z (starting from y to respect ordering) */
            for (idx_t z = y;; ++z)
            {
                seq_t z2 = (seq_t)z * (seq_t)z;
                if (z2 > t1)
                    break;

                seq_t t2 = t1 - z2;            /* value that must equal w² */

                seq_t w = floor_sqrt(t2);     /* integer square root of t2 */
                if (w * w == t2 && w >= (seq_t)z)
                    ++count;
            }
        }
    }

    return count;
}
