/*
 * A326401  Expansion of Sum_{k>=1} k * x^k / (1 + x^k + x^(2*k)).
 * https://oeis.org/A326401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n, n/d==1 (mod 3)} d – Sum_{d|n, n/d==2 (mod 3)} d
 * Range:     u8 n <= 16, u16 n <= 5467, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* safety, offset is 1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);    /* factorize n */

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p     = (seq_t)primes[i];
        idx_t  e    = exps[i];
        seq_t part;

        if (primes[i] == 3) {                        /* a(3^e) = 3^e */
            part = big_pow(p, (seq_t)e);
        } else if ((primes[i] % 3) == 1) {           /* p ≡ 1 (mod 3) */
            seq_t pow_  = big_pow(p, (seq_t)(e + 1));
            seq_t numer = pow_ - 1;
            seq_t denom = p - 1;
            part = numer / denom;
        } else { /* p ≡ 2 (mod 3) */
            seq_t pow_  = big_pow(p, (seq_t)(e + 1));
            seq_t numer;
            if ((e & 1) == 0)                      /* e even: (+1) */
                numer = pow_ + 1;
            else                                   /* e odd: (-1) */
                numer = pow_ - 1;
            seq_t denom = p + 1;
            part = numer / denom;
        }
        result *= part;
    }
    return result;
}
