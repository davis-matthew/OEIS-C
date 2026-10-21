/*
 * A276385  Defined by the properties that it starts with 2, and when you
 *          successively apply DIFF, RUNS, BISECT, RUNS you get (4,1,3,1)
 *          repeated infinitely often.
 * https://oeis.org/A276385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n * t) where t is the tribonacci constant, the
 *            positive solution of x^3 = x^2 + x + 1
 * Range:     u8 n <= 90, u16 n <= 9927, u32 n <= 9927, u64 n <= 9927,
 *            u128 n <= 9927, big any n
 * Helpers:   floor_nt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_nt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                  /* value below the offset – undefined */

    seq_t ft = floor_nt(n);               /* floor(n * t)          */
    return (seq_t)n + ft;                 /* a(n) = n + floor(n*t) */
}
