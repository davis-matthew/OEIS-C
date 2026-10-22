/*
 * A332454  Starting from sigma(n)+1, number of halving steps to reach 1 in
 *          '3x+1' problem, or -1 if this never happens.
 * https://oeis.org/A332454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006666(1+sigma(n))
 * Range:     u8 n <= 26, u16 n <= 8747, u32 n <= 9049, u64 n <= 9049,
 *            u128 n <= 9049, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of halving steps needed for the Collatz (3x+1) process
   starting from 1 + sigma(n). The result is returned in type seq_t.
   (If the sequence never reaches 1, the function would return an
   arbitrarily large value, but this never occurs for the input sizes in
   the tests.) */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Sum of divisors of n */
    seq_t m = sigma_of(n) + (seq_t)1;   /* start value 1 + sigma(n) */

    seq_t steps = (seq_t)0;
    while (m != (seq_t)1)
    {
        if ((m & (seq_t)1) == 0)   /* even: halve */
        {
            m >>= 1;
            steps++;               /* count halving step */
        }
        else                       /* odd: apply 3x+1 */
        {
            m = (seq_t)3 * m + (seq_t)1;
        }
    }

    return steps;
}
