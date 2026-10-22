/*
 * A395774  The twisted Euler phi-function for the Dirichlet character
 *          kronecker(-19,.) mod 19.
 * https://oeis.org/A395774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative function defined by a(19^e) = 19^e, a(p^e)
 *            = p^(e-1)*(p-1) if p ≡ 1,4,5,6,7,9,11,16,17 (mod 19), a(p^e) =
 *            p^(e-1)*(p+1) otherwise
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* simple integer power, exponent fits in a small idx_t */
static inline seq_t pow_uint64(uint64_t base, idx_t exp)
{
    seq_t res = 1;
    seq_t b   = (seq_t)base;
    for (idx_t i = 0; i < exp; ++i)
        res *= b;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, safe fallback */

    /* maximum number of distinct prime factors of a 64‑bit integer < 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 19)
        {
            result *= pow_uint64(19, e);
            continue;
        }

        seq_t p_pow = pow_uint64(p, (idx_t)(e - 1));
        uint64_t r  = p % 19;

        seq_t factor;
        if (r == 1 || r == 4 || r == 5 || r == 6 || r == 7 ||
            r == 9 || r == 11 || r == 16 || r == 17)
            factor = (seq_t)(p - 1);
        else
            factor = (seq_t)(p + 1);

        result *= p_pow;
        result *= factor;
    }

    return result;
}
