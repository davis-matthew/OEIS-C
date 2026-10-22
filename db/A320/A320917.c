/*
 * A320917  a(n) = sigma_2(n)*sigma_3(n)/sigma(n).
 * https://oeis.org/A320917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma_2(n) * sigma_3(n) / sigma(n)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute base^exp using 128‑bit integers. */
static inline __int128 pow_uint128(uint64_t base, unsigned exp)
{
    __int128 res = 1;
    for (unsigned i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for 0, just in case */

    __int128 total = 1;     /* running product of contributions */

    /* factor_u64(n, primes, exps, &cnt) fills the arrays with the prime
       factorisation of n.  For n <= 10000 at most 5 distinct primes occur,
       so fixed size arrays are safe. */
    uint64_t primes[8];
    uint64_t exps[8];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        unsigned e = (unsigned)exps[i];          /* exponent of p */

        /* σ(p^e) = (p^(e+1) - 1) / (p - 1) */
        __int128 p_e = pow_uint128(p, e);           /* p^e */
        __int128 p_e1 = p_e * p;                   /* p^(e+1) */
        __int128 sigma1 = (p_e1 - 1) / (p - 1);

        /* σ₂(p^e) = (p^{2(e+1)} - 1) / (p² - 1) */
        __int128 sigma2 = (pow_uint128(p, 2 * (e + 1)) - 1) /
                          ( ( (__int128)p * p ) - 1 );

        /* σ₃(p^e) = (p^{3(e+1)} - 1) / (p³ - 1) */
        __int128 sigma3 = (pow_uint128(p, 3 * (e + 1)) - 1) /
                          ( ( (__int128)p * p * p ) - 1 );

        /* contribution of this prime power */
        __int128 contrib = sigma2 * sigma3 / sigma1;
        total *= contrib;
    }

    return (seq_t)total;
}
