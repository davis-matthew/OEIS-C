/*
 * A322966  Denominator of Sum_{d | n} 1/rad(d) where rad = A007947.
 * https://oeis.org/A322966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of Sum_{d|n} 1/rad(d), where rad is the
 *            product of distinct prime factors. The value is computed
 *            multiplicatively via f(p^k) = (p + k) / p and the result reduced
 *            to lowest terms
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t num = 1;
    seq_t den = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p  = (seq_t)primes[i];
        seq_t k  = (seq_t)exps[i];
        seq_t add = p + k;          /* numerator part of (1 + k/p) */
        num = num * add;
        den = den * p;

        /* Reduce fraction to keep numbers small */
        seq_t g = gcd(num, den);
        if (g > 1) {
            num = num / g;
            den = den / g;
        }
    }

    return den;
}
