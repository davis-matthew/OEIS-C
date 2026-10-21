/*
 * A257840  y-value of the lexicographically first integer solution (x,y,z) of
 *          4/n = 1/x + 1/y + 1/z with 0 < x < y < z, or 0 if there is no such
 *          solution. Corresponding x and z values are in A257839 and A257841.
 * https://oeis.org/A257840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 514, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the calculation of A257840: for a given n, find the
   y-value of the lexicographically first integer solution (x,y,z)
   of 4/n = 1/x + 1/y + 1/z with 0 < x < y < z, or 0 if no solution exists */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    idx_t N = n;
    /* x must satisfy n/4 < x <= 3n/4 to allow a positive 1/x term */
    idx_t x_start = N / 4 + 1;
    idx_t x_end   = (3 * N) / 4;

    for (idx_t x = x_start; x <= x_end; ++x)
    {
        /* 4x - n > 0 for these x */
        idx_t coeff = 4 * x - N;

        /* minimal y such that the denominator becomes positive */
        idx_t y_min = N * x / coeff + 1;
        if (y_min <= x) y_min = x + 1;   /* enforce y > x */

        /* maximal y so that z > y; derived from y*(4x - n) < 2 n x */
        idx_t y_max = (2 * N * x - 1) / coeff;

        if (y_min > y_max) continue;

        for (idx_t y = y_min; y <= y_max; ++y)
        {
            idx_t denom = y * coeff - N * x;   /* 4xy - ny - nx */
            if (denom == 0) continue;

            idx_t num_xy = N * x * y;          /* nxy */

            if (num_xy % denom != 0) continue; /* z must be integer */

            idx_t z = num_xy / denom;         /* z = num_xy / denom */

            if (z > y)                       /* ensures x < y < z */
                return (seq_t)y;              /* lexicographically first */
        }
    }

    return (seq_t)0; /* no solution */
}
