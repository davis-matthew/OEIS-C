/*
 * A259846  Number of steps to get from n to 1 by process of replacing with
 *          sum of divisors if the result is odd, or dividing by 2 if even.
 * https://oeis.org/A259846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A259846: number of steps required to reduce n to 1 by the
 * following process: if the current value is even, replace it by the
 * value divided by two; if it is odd, replace it by the sum of its
 * positive divisors σ(n).  a(1) = 0.  This implementation uses the
 * provided sigma_of utility, which returns a seq_t value, and then
 * casts the result back to idx_t for subsequent iterations.
 */
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t steps = 0;
    while (n != 1)
    {
        if ((n & 1ULL) == 0)        /* even: divide by 2 */
            n >>= 1;
        else                        /* odd: replace with σ(n) */
            n = (idx_t)sigma_of(n);
        ++steps;
    }
    return steps;
}
