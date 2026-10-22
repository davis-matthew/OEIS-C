/*
 * A319995  Number of divisors of n of the form 6*k + 5.
 * https://oeis.org/A319995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n which are congruent to 5 mod 6
 * Range:     u8 n <= 825, u16 n <= 7380, u32 n <= 7380, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* External linkage function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* out-of-bounds guard, though offset starts at 1 */

    seq_t count = 0;

    /* iterate over all divisors up to sqrt(n) */
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* d is a divisor */
            if ((seq_t)d % 6 == 5)
                ++count;

            /* paired divisor n/d, avoid double counting when d==n/d */
            idx_t d2 = n / d;
            if (d2 != d && (seq_t)d2 % 6 == 5)
                ++count;
        }
    }

    return count;
}
