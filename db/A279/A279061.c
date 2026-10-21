/*
 * A279061  Number of divisors of n of the form 7*k + 1.
 * https://oeis.org/A279061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n congruent to 1 mod 7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a direct check of all divisors of n. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* by definition a(0) = 0 */

    seq_t count = 0;

    /* Enumerate all d that divide n and satisfy d % 7 == 1 */
    for (idx_t d = 1; d <= n; ++d)
    {
        if (d % 7 == 1 && n % d == 0)
            count++;
    }

    return count;
}
