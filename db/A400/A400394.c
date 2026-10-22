/*
 * A400394  a(n) = Sum_{d|n} d * gcud(d, n/d), where gcud is the greatest
 *          common unitary divisor.
 * https://oeis.org/A400394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} d * gcud(d, n/d) (Equivalently, a is
 *            multiplicative with a(p^e) = sigma(p^e) if e odd sigma(p^e) +
 *            p^e - p^(e/2) if e even )
 * Range:     u8 n <= 16, u16 n <= 2216, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

static seq_t sigma_of_prime_power(uint64_t p, uint64_t e)
{
    /* sigma(p^e) = (p^{e+1} - 1)/(p-1) */
    seq_t pe1 = big_pow(p, e + 1);
    seq_t num = pe1 - 1;
    seq_t den = (seq_t)(p - 1);
    return num / den;
}

static seq_t a_prime_power(uint64_t p, uint64_t e)
{
    seq_t sigma = sigma_of_prime_power(p, e);
    if (e & 1ULL)                /* e is odd */
        return sigma;
    /* e even */
    seq_t pe  = big_pow(p, e);
    seq_t hin = big_pow(p, e / 2);
    return sigma + pe - hin;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, but safe */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
        result *= a_prime_power(primes[i], exps[i]);

    return result;
}
