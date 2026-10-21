/*
 * A284151  Sum_{d|n, d=1 or 6 mod 7} d.
 * https://oeis.org/A284151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all divisors d of n such that d ≡ 1 or 6 (mod 7)
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 we simply return 0. */
    if (n == 0)
        return 0;

    seq_t sum = 0;          /* result accumulator */
    idx_t sqrt_n = 0;       /* floor(sqrt(n)) */

    /* Compute floor(sqrt(n)) without overflow.  We avoid i*i because
       (idx_t)i might exceed the range of idx_t if n is large. */
    for (idx_t i = 1; i <= n / i; ++i)
        sqrt_n = i;

    /* Enumerate divisors up to sqrt(n) and add those in the desired residue
       classes.  Each divisor d has a complementary divisor e = n / d. */
    for (idx_t d = 1; d <= sqrt_n; ++d)
    {
        if (n % d == 0)
        {
            idx_t e = n / d;

            /* Add d if it satisfies the condition. */
            if (d % 7 == 1 || d % 7 == 6)
                sum += (seq_t)d;

            /* Add the complementary divisor e if it is distinct from d
               and satisfies the condition. */
            if (e != d && (e % 7 == 1 || e % 7 == 6))
                sum += (seq_t)e;
        }
    }

    return sum;
}
