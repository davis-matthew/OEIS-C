/*
 * A400248  Dirichlet convolution of A085731 with A055615 (Dirichlet inverse
 *          of n), where A085731(n) = gcd(n,n'), and n' is the arithmetic
 *          derivative of n.
 * https://oeis.org/A400248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Sum_{d|n} A055615(n/d)*A085731(d) (Dirichlet convolution
 *            of A085731 with A055615, multiplicative with a(p^e) = p^e -
 *            p^(e-1) if e%p==0, a(p^e) = p^(e-1) - p^e if (e-1)%p==0,
 *            otherwise 0)
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t ppow = (seq_t)1;
        for (uint64_t k = 0; k < e; ++k) {
            ppow *= (seq_t)p;
        }

        seq_t val;
        if (e % p == 0) {
            val = ppow - ppow / (seq_t)p;
        } else if ((e - 1) % p == 0) {
            val = ppow / (seq_t)p - ppow;
        } else {
            val = (seq_t)0;
        }

        if (val == (seq_t)0)
            return (seq_t)0;   /* product becomes zero */

        result *= val;
    }
    return result;
}
