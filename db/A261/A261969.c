/*
 * A261969  Product of primes dividing n with maximum multiplicity.
 * https://oeis.org/A261969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of primes dividing n whose exponent is the
 *            maximum exponent in n's prime factorization
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* a(1) = 1 by convention */

    uint64_t nn = (uint64_t)n;    /* working copy of n */
    uint32_t primes[10];
    uint32_t exps[10];
    uint32_t cnt = 0;
    uint32_t mx = 0;

    /* factorize n by trial division */
    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            uint32_t exp = 0;
            while (nn % p == 0) {
                nn /= p;
                ++exp;
            }
            primes[cnt] = (uint32_t)p;
            exps[cnt] = exp;
            if (exp > mx)
                mx = exp;
            ++cnt;
        }
    }
    if (nn > 1) {          /* remaining prime factor */
        primes[cnt] = (uint32_t)nn;
        exps[cnt] = 1;
        if (1 > mx)
            mx = 1;
        ++cnt;
    }

    /* compute product of primes with exponent equal to mx */
    seq_t result = 1;
    for (uint32_t i = 0; i < cnt; ++i) {
        if (exps[i] == mx)
            result *= (seq_t)primes[i];
    }
    return result;
}
