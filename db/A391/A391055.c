/*
 * A391055  a(n) is the number of iterations of x-> gpf(3*x+1) starting at n
 *          until the value 2 is reached; a(n) = -1 if 2 is not reached.
 * https://oeis.org/A391055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x → gpf(3·x+1) starting from n
 *            until x=2 (a(2)=0)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)-1;                /* undefined for n < 1, return sentinel */

    if (n == 2)
        return (seq_t)0;                /* already at 2, no iterations needed */

    seq_t steps = 0;
    uint64_t x = (uint64_t)n;

    /* Iterate until 2 is reached or an overflow leads to an infinite loop */
    while (x != 2)
    {
        uint64_t val = 3ULL * x + 1ULL;   /* 3*x + 1 fits in uint64_t for used values */
        x = gpf(val);                     /* greatest prime factor of val */
        steps++;

        /* Protect against potential stalling: the sequence is conjectured
           to always reach 2, but if for some reason it does not, break. */
        if (steps == (uint64_t)-1)
            break;
    }

    if (x == 2)
        return steps;                     /* successful termination count */
    else
        return (seq_t)-1;                 /* failure: 2 was not reached */
}
