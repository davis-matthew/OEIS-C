/*
 * A392329  a(n) is the number of iterations x->gpf(6*x+1) starting at n until
 *          the value 13 or 47 is reached; a(n) = -1 if neither of these two
 *          values is reached.
 * https://oeis.org/A392329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of iterations of x -> gpf(6*x+1)
   from n until the value 13 or 47 is reached; a(13)=a(47)=0. */

#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;     /* current value in the iteration */
    seq_t  steps = 0;              /* number of iterations performed */

    /* iterate until 13 or 47 is reached */
    while (x != 13ULL && x != 47ULL)
    {
        uint64_t val = 6ULL * x + 1ULL;  /* compute 6*x+1 (fits in 64 bits for test range) */
        x = gpf(val);                    /* largest prime factor */
        steps = steps + 1;               /* increment step counter */
    }
    return steps;
}
