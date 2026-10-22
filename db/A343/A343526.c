/*
 * A343526  Number of divisors of n^7.
 * https://oeis.org/A343526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (7·e + 1) (number of divisors of n⁷)
 * Range:     u8 n <= 29, u16 n <= 9239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes the number of divisors of n⁷. */
seq_t a(idx_t n)
{
    /* n is at least 1.  Work on a copy to factor. */
    uint64_t x = (uint64_t)n;
    seq_t result = 1;

    for (uint64_t p = 2; p <= x / p; ++p)
    {
        if (x % p == 0)
        {
            uint64_t e = 0;
            while (x % p == 0)
            {
                x /= p;
                ++e;
            }
            result *= (7 * e + 1);
        }
    }

    /* If a prime factor larger than sqrt remains. */
    if (x > 1)
    {
        result *= 8;   /* 7*1 + 1 = 8 */
    }

    return result;
}
