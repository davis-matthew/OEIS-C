/*
 * A369758  The sum of divisors of the smallest cubefull exponentially odd
 *          number that is divisible by n.
 * https://oeis.org/A369758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma( m ) where m = product over prime factors p of n of
 *            p^f, with f being the smallest odd integer >= max(e,3) (e =
 *            exponent of p in n)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 1108, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               // undefined for n=0, safety

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;                    // sigma(1) = 1

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* determine f: odd exponent >= max(e,3) */
        uint64_t f = e;
        if (f < 3)          f = 3;
        if ((f & 1) == 0)   f++;                /* make odd */

        /* sigma(p^f) = (p^(f+1)-1)/(p-1) */
        seq_t num = big_pow(p, (seq_t)(f + 1)) - (seq_t)1;
        seq_t den = (seq_t)(p - 1);
        seq_t sigma = num / den;

        result *= sigma;
    }

    return result;
}
