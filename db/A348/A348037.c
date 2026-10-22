/*
 * A348037  a(n) = n / gcd(n, A003968(n)), where A003968 is multiplicative
 *          with a(p^e) = p*(p+1)^(e-1).
 * https://oeis.org/A348037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / gcd(n, A003968(n)) where A003968(n) is multiplicative
 *            with a(pᵉ) = p·(p+1)^(e−1)
 * Range:     u8 n <= 511, u16 n <= 9557, u32 n <= 9557, u64 n <= 9557,
 *            u128 n <= 9557, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The code factors n, then for each prime p computes the exponent of p
   in A003968(n) as 1 + Σ_{q≠p} (e_q−1)·v_p(q+1)   (v_p = p‑adic valuation).
   The exponent of p in gcd(n, A003968(n)) is min(e_p, exponent_in_A).
   Finally a(n) = Π p^{e_p−min(e_p,exponent_in_A)}.  */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static inline seq_t pow_seq_t(seq_t base, unsigned long long exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;               /* a(1) = 1 */

    /* Work with 64‑bit values for prime factors and exponents */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t cnt = 0;

    /* factor n into primes */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* exponent of p in A003968(n) */
        unsigned long long exp_in_A = 1ULL;          /* from the p factor itself */

        for (size_t j = 0; j < cnt; ++j)
        {
            if (j == i)
                continue;                            /* skip same prime */

            uint64_t q = primes[j];
            uint64_t eq = exps[j];
            if (eq <= 1ULL)
                continue;                           /* (eq-1) == 0 */

            unsigned long long sub = eq - 1ULL;
            uint64_t t = q + 1ULL;
            unsigned v = 0ULL;
            while (t % p == 0ULL)
            {
                ++v;
                t /= p;
            }
            exp_in_A += sub * v;
        }

        unsigned long long g_exp = (e < exp_in_A) ? e : exp_in_A;
        unsigned long long rem_exp = e - g_exp;
        if (rem_exp > 0ULL)
            result *= pow_seq_t((seq_t)p, rem_exp);
    }

    return result;
}
