/*
 * A258409  Greatest common divisor of all (d-1)'s, where the d's are the
 *          positive divisors of n.
 * https://oeis.org/A258409
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd_{d|n} (d-1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* Enumerate all divisors of n using its prime factorisation.  The
   divisors are generated recursively from the prime bases in primes[]
   and their exponents in exps[].  The running gcd of (d-1) values
   is accumulated in *g.  */
static void _enum_divisors(size_t idx,
                           uint64_t cur,
                           const uint64_t *primes,
                           const uint64_t *exps,
                           size_t cnt,
                           seq_t *g)
{
    if (idx == cnt) {
        seq_t d = (seq_t)cur;
        seq_t val = d - (seq_t)1;
        if (*g == (seq_t)0)
            *g = val;
        else
            *g = gcd(*g, val);
        return;
    }

    uint64_t p = primes[idx];
    uint64_t prod = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        _enum_divisors(idx + 1, prod, primes, exps, cnt, g);
        prod *= p;
    }
}

seq_t a(idx_t n)
{
    /* For even n the divisor 2 gives 2-1 = 1, forcing the gcd to be 1. */
    if ((n & 1) == 0)
        return (seq_t)1;

    /* Factorise the odd n.  Only primes and exponents are needed. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the gcd of all (d-1). */
    seq_t g = (seq_t)0;
    _enum_divisors(0, (uint64_t)1, primes, exps, cnt, &g);
    return g;
}
