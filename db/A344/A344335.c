/*
 * A344335  Number of divisors of n^8.
 * https://oeis.org/A344335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of divisors of n⁸ = ∏_{p⁽ᵉ⁾‖n} (8·e + 1)
 * Range:     u8 n <= 29, u16 n <= 4619, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    seq_t result = 1;                 /* multiplicative identity */

    /* trial‑division factorization */
    for (uint64_t p = 2; p <= x / p; ++p)
    {
        if (x % p != 0) continue;

        size_t exp = 0;
        while (x % p == 0)
        {
            x  /= p;
            ++exp;
        }
        result *= (seq_t)(8 * exp + 1);
    }

    /* If a prime factor > sqrt(original n) remains */
    if (x > 1)
    {
        result *= (seq_t)(8 * 1 + 1);
    }

    return result;
}
