/* factorize: the prime factorization of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_FACTORIZE_H
#define OEISC_UTILS_FACTORIZE_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static size_t factorize(uint64_t n, uint64_t primes[], int exps[])
{
    size_t cnt = 0;
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            primes[cnt] = p;
            exps[cnt] = e * 2;          /* exponents doubled for n² */
            cnt++;
        }
    }
    if (n > 1) {
        primes[cnt] = n;
        exps[cnt]   = 2;                /* exponent doubled */
        cnt++;
    }
    return cnt;
}

#endif
