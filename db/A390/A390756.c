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
 * Validated: standard, pass, 64 terms through n = 8239 (2026-11-01)
 * Helpers:   ipow_u64_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_u64_i32.h"

/* factor contribution for prime power p^e */
static seq_t _factor_contrib(uint64_t p, int e)
{
    seq_t part = (seq_t)p - 1;
    if (e < 3)
    {
        /* (p-1) * p² */
        return part * (seq_t)p * (seq_t)p;
    }
    else
    {
        /* (p-1) * p^(e-1) */
        seq_t powp = ipow_u64_i32(p, e - 1);
        return part * powp;
    }
}

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
