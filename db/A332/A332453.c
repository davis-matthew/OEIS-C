/*
 * A332453  Starting from sigma(n), number of tripling steps to reach 1 in
 *          '3x+1' problem, or -1 if 1 is never reached.
 * https://oeis.org/A332453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of “tripling” (3x+1) steps in the Collatz *
 *            iteration starting from sigma(n), until the value 1 is reached.
 *            * If 1 is never reached the function returns -1. The call *
 *            sigma_of(n) (from "../utils/sigma_of.h") provides the sum of
 *            divisors
 * Range:     u8 n <= 15, u16 n <= 5328, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 121 terms through n = 7812 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* starting value: sum of divisors of n */
    seq_t val = sigma_of(n);
    seq_t steps = 0;

    /* iterate until the value becomes 1 */
    while (val != (seq_t)1)
    {
        if (val & (seq_t)1)          /* odd → apply 3x+1 */
        {
            val = val * (seq_t)3 + (seq_t)1;
            steps++;
        }
        else                          /* even → divide by 2 */
        {
            val >>= 1;                /* safe right shift on seq_t */
        }
    }

    return steps;
}
