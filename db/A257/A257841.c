/*
 * A257841  z-value of the lexicographically first solution (x,y,z) of 4/n =
 *          1/x + 1/y + 1/z with 0 < x < y < z all integers, or 0 if there is
 *          no such solution. Corresponding x and y values are in A257839 and
 *          A257840.
 * https://oeis.org/A257841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 34, u32 n <= 514, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) : z‑value of the lexicographically first solution (x,y,z) of
 *  4/n = 1/x + 1/y + 1/z with 0 < x < y < z all integers;
 *  returns 0 if no such solution.
 *
 *  The algorithm iterates over possible x from ceil(n/4)+1 up to floor(3n/4)
 *  (because x must satisfy 3/x >= 4/n).
 *  For each x it finds the smallest y> x (bounded by y < 2nx/(4x-n))
 *  such that (y*n*x) is divisible by ((4x-n)*y - n*x).  The resulting
 *  z is then checked to be > y.  The first z found is returned.
 *
 *  Complexity : O(n·M) with M ≈ n/2 in the worst case, fast enough for
 *  n up to 10 000.  All arithmetic uses seq_t only, no floating point.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                       /* 1, 2 have no solution */

    idx_t start = n / 4 + 1;            /* smallest possible x */
    idx_t end   = (3 * n) / 4;          /* upper bound for x */

    for (idx_t x = start; x <= end; ++x) {
        seq_t sx = (seq_t)x;
        seq_t sn = (seq_t)n;
        seq_t num_x = sn * sx;               /* n*x */
        idx_t denom_const_int = 4 * x - n;   /* 4x - n > 0 by construction */
        if (denom_const_int <= 0)
            continue;
        seq_t delta = (seq_t)denom_const_int;/* 4x - n */

        /* Compute maximal y from y < 2nx/(4x-n)          */
        seq_t y_max_seq = ( (seq_t)2 * sn * sx ) / delta;
        idx_t y_max = (idx_t)y_max_seq;        /* safe cast, value fits in idx_t */

        if (y_max <= x)
            continue;

        for (idx_t y = x + 1; y <= y_max; ++y) {
            seq_t sy = (seq_t)y;
            seq_t den = delta * sy - num_x;    /* (4x-n)y - n*x */
            if (den <= 0)
                continue;
            seq_t num = sy * sn * sx;          /* y*n*x */
            if (num % den == 0) {
                seq_t z = num / den;
                if (z > sy)  /* ensures 0 < x < y < z */
                    return z;
            }
        }
    }
    return 0;   /* no representation found */
}
