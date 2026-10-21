/*
 * A246601  Sum of divisors d of n with property that the binary
 *          representation of d can be obtained from the binary representation
 *          of n by changing any number of 1's to 0's.
 * https://oeis.org/A246601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{d|n, (d & n) == d} d
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently, sum of all divisors d of n such that d is a submask of n. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Zero‑based edge case: the sequence is defined for n ≥ 1, but
       to keep the function well defined we simply return 0 for n = 0. */
    if (n == 0)
        return 0;

    seq_t sum = 0;

    /* Enumerate divisors up to sqrt(n).  Avoid overflow by comparing
       i with n / i. */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0)
            continue;

        idx_t d1 = i;
        idx_t d2 = n / i;

        /* Check the submask condition for each divisor. */
        if ((d1 & n) == d1)
            sum += (seq_t)d1;

        if (d2 != d1) {
            if ((d2 & n) == d2)
                sum += (seq_t)d2;
        }
    }

    return sum;
}
