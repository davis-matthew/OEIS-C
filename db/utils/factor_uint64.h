/* factor_uint64: the prime factorization of a 64-bit n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_FACTOR_UINT64_H
#define OEISC_UTILS_FACTOR_UINT64_H
#include <stdint.h>

/* factor a positive integer x into prime factors (at most 20 distinct primes).
   Stores primes in fac[], exponents in exp[], and number of distinct primes in *cnt. */
static void factor_uint64(uint64_t x, uint64_t fac[20], int exp[20], int *cnt)
{
    *cnt = 0;
    for (uint64_t d = 2; d * d <= x && *cnt < 20; ++d) {
        if (x % d == 0) {
            int e = 0;
            while (x % d == 0) {
                x /= d;
                ++e;
            }
            fac[(*cnt)]   = d;
            exp[(*cnt)]   = e;
            ++(*cnt);
        }
    }
    if (x > 1 && *cnt < 20) {
        fac[(*cnt)]   = x;
        exp[(*cnt)]   = 1;
        ++(*cnt);
    }
}

#endif
