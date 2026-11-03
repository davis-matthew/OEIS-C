/*
 * A346392  a(n) is the number of proper divisors of n ending with the same
 *          digit as n.
 * https://oeis.org/A346392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |{ d | d|n, d≠n, d mod 10 = n mod 10 }|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9984 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of proper divisors of n ending with the same
 * decimal digit as n.  Uses an O(√n) divisor enumeration. */
#include <stdint.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;              /* 1 has no proper divisors */

    seq_t count = 0;
    idx_t last = n % 10;                  // last digit of n
    idx_t max_i = isqrt(n);    // largest i with i*i ≤ n

    for (idx_t i = 1; i <= max_i; ++i)
    {
        if (n % i == 0)                   // i is a divisor
        {
            if (i != n && (i % 10) == last)
                ++count;

            idx_t j = n / i;              // complementary divisor
            if (j != i && j != n && (j % 10) == last)
                ++count;
        }
    }
    return count;
}
