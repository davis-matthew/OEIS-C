/*
 * A384713  The number of steps that n requires to reach 1 under the map: x->
 *          x^2 - 1 if x is an odd prime, x/2 if x is even, x - lpf(x)
 *          otherwise where lpf(x) is the least prime factor of x. a(n) = -1
 *          if 1 is never reached.
 * https://oeis.org/A384713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2055, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A384713: the number of steps for n to reach 1 under
 * the map
 *   x -> x²-1   if x is an odd prime,
 *   x -> x/2    if x is even,
 *   x -> x - lpf(x)  otherwise          (lpf is least prime factor).
 * Returns (seq_t)-1 if 1 is not reached within an imposed safe limit.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/lpf.h"

seq_t a(idx_t n)
{
    /* Work in terms of the numeric type of the sequence value. */
    seq_t x = (seq_t)n;
    seq_t steps = 0;

    /* Safety guard: limit the number of iterations to avoid accidental
     * infinite cycles, though for the explored range this never
     * triggers. */
    const seq_t max_steps = (seq_t)1000000;

    while (x != (seq_t)1) {
        /* Even numbers: divide by two. */
        if ((x & (seq_t)1) == 0) {
            x = x / (seq_t)2;
        } else {
            /* Odd x: determine whether it is prime. */
            idx_t spf = lpf((idx_t)x);          /* least prime factor */
            if ((idx_t)spf == (idx_t)x) {       /* odd prime */
                x = x * x - (seq_t)1;
            } else {                            /* odd composite */
                x = x - (seq_t)spf;
            }
        }

        ++steps;
        if (steps > max_steps)
            return (seq_t)-1;
    }
    return steps;
}
