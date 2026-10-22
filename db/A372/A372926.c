/*
 * A372926  a(n) = Sum_{1 <= x_1, x_2 <= n} gcd(x_1, x_2, n)^4.
 * https://oeis.org/A372926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{1 <= x1,x2 <= n} gcd(x1,x2,n)^4 Computation via
 *            multiplicativity: a(p^e) = p^(2e-2) * (p^(2e)*(p^2+1) - 1) a(n)
 *            = ∏_p a(p^e)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 239, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static inline __int128 pow_int128(uint64_t base, unsigned int exp)
{
    __int128 res = 1;
    __int128 b = base;
    while (exp) {
        if (exp & 1) res *= b;
        b *= b;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned int e = (unsigned int)exps[i];

        __int128 p2   = (__int128)p * p;              /* p^2           */
        __int128 pow2e = pow_int128(p, 2 * e);        /* p^(2e)        */
        __int128 term  = pow2e * (p2 + 1) - 1;        /* p^(2e)*(p^2+1)-1 */
        __int128 factor = pow_int128(p, (unsigned int)(2 * e - 2)); /* p^(2e-2) */
        __int128 ap = factor * term;                 /* a(p^e)        */

        /* Multiply into result using 128‑bit intermediate to avoid overflow */
        __int128 tmp = (__int128)result * ap;
        result = (seq_t)tmp;
    }
    return result;
}
