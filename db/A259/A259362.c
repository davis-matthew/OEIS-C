/*
 * A259362  a(1) = 1, for n > 1: a(n) is the number of ways to write n as a
 *          nontrivial perfect power.
 * https://oeis.org/A259362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) counts the unordered pairs (i , j) with i > 0, j > 1 such
 *            that n = i^j. Equivalently, a(n) is the number of distinct bases
 *            i for which n is a non‑trivial perfect power
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the fact that n = i^j iff all exponents in the
 * prime factorisation of n are divisible by j.  Hence j must be a divisor
 * of the gcd of those exponents.  Each such j yields one base
 * i = n^(1/j).  For n = 1 we return 1 by definition. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static uint64_t local_gcd(uint64_t a, uint64_t b)
{
    while (b != 0)
    {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* Factor n; primes and exps will be filled. */
    factor_u64(nn, primes, exps, &cnt);

    /* Compute the gcd of all exponents.  We know cnt >= 1 for n > 1. */
    uint64_t g = exps[0];
    for (size_t i = 1; i < cnt; ++i)
        g = local_gcd(g, exps[i]);

    /* Count divisors of g that are >= 2.  Each gives one distinct base. */
    seq_t result = 0;
    for (uint64_t d = 2; d <= g; ++d)
    {
        if (g % d == 0)
            ++result;
    }

    return result;
}
