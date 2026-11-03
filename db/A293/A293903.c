/*
 * A293903  Sum of proper divisors of n of the form 4k+3.
 * https://oeis.org/A293903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d<n} [3 == d mod 4] * d
 * Range:     u8 n <= 429, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9924 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0; /* no proper divisors for 1 */

    idx_t lim = isqrt(n);      /* floor(sqrt(n)) */
    seq_t sum = 0;

    for (idx_t d = 1; d <= lim; ++d)
    {
        if (n % d == 0)
        {
            if (d != n && (d & 3) == 3)
                sum += (seq_t)d;

            idx_t other = n / d;
            if (other != d && other != n && (other & 3) == 3)
                sum += (seq_t)other;
        }
    }

    return sum;
}
