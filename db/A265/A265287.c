/*
 * A265287  Number of steps needed to reach 1 in the "sqrt(2)*x + 1" problem.
 * https://oeis.org/A265287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps to reach 1 in the sqrt(2)*x + 1 problem
 *            (starting with x = n)
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 516, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t steps = 0;
    while (x > 1)
    {
        if ((x & 1) == 0)
        {
            x >>= 1;                 /* even case: divide by 2 */
        }
        else
        {
            /* odd case: floor(x*sqrt(2) + 1)
               Integer part of x*sqrt(2) is floor(sqrt(2*x*x)) */
            seq_t t = x * x;
            t = t * 2;
            x = floor_sqrt(t) + 1;
        }
        steps++;
    }
    return steps;
}
