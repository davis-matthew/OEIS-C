/*
 * A349877  a(n) is the number of times the map x -> A353314(x) needs to be
 *          applied to n to reach a multiple of 3, or -1 if the trajectory
 *          never reaches a multiple of 3.
 * https://oeis.org/A349877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of iterations of f(x) = floor((5*x + 7) / 3) until
 *            x becomes a multiple of 3, or -1 if this never occurs
 * Range:     u8 n <= 1, u16 n <= 3246, u32 n <= 9737, u64 n <= 9737,
 *            u128 n <= 9737, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* external linkage, sig. provided by the build environment */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;

    /* if already a multiple of 3, zero iterations */
    if (x % 3 == 0)
        return 0;

    /* iterate until a multiple of 3 is found */
    seq_t steps = 0;
    /* the mapping strictly increases, so we use a generous upper bound
       to guard against accidental infinite loops.  For the ranges tested
       in the OEIS data this bound is never reached. */
    const idx_t MAX_STEPS = 1000000;

    while (steps < MAX_STEPS)
    {
        /* compute the next value according to A353314:
           f(x) = floor((5*x + 7) / 3)  (integer division truncates) */
        x = (5 * x + 7) / 3;
        steps++;

        if (x % 3 == 0)
            return steps;
    }

    /* if the loop never reaches a multiple of 3, return -1
       represented in an unsigned type as the maximal value. */
    return (seq_t)-1;
}
