/*
 * A352038  Sum of the 10th powers of the odd proper divisors of n.
 * https://oeis.org/A352038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d<n, d odd} d^10
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 21, u64 n <= 169,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Sum of the 10th powers of all odd proper divisors of n.  */
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* First divisor d. */
            if (d < n && (d & 1uL))
                sum += big_pow((seq_t)d, (seq_t)10);

            /* Complement divisor n/d. */
            idx_t d2 = n / d;
            if (d2 != d && d2 < n && (d2 & 1uL))
                sum += big_pow((seq_t)d2, (seq_t)10);
        }
    }
    return sum;
}
