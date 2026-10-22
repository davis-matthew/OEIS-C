/*
 * A390756  The Euler totient of the smallest cubefull number divisible by n.
 * https://oeis.org/A390756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   A390756 is multiplicative. For n = ∏ pᶠ, a(n) = ∏ (p‑1)pᵏ where
 *            k = 2 if f < 3, and k = f‑1 if f ≥ 3. This is equivalent to a(n)
 *            = φ(A356193(n))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _factor_contrib, _prime_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_factor_contrib.h"
#include "../utils/_prime_pow.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t tmp = n;

    /* factor 2 separately */
    int e = 0;
    while ((tmp & 1ULL) == 0 && tmp > 0)
    {
        tmp >>= 1;
        ++e;
    }
    if (e > 0)
        result *= _factor_contrib(2, e);

    /* odd primes */
    for (uint64_t p = 3; p * p <= tmp; p += 2)
    {
        if (tmp % p == 0)
        {
            e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            result *= _factor_contrib(p, e);
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1)
        result *= _factor_contrib(tmp, 1);

    return result;
}
