/* sigma_u64_to_u64: the sum of the divisors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_SIGMA_U64_TO_U64_H
#define OEISC_UTILS_SIGMA_U64_TO_U64_H
#include <stdbool.h>
#include <stdint.h>

static uint64_t sigma_u64_to_u64(uint64_t m) {
    uint64_t sum = 1;     /* multiplicative identity */
    if (m == 1) return 1; /* σ(1)=1 */

    /* factor out 2 */
    if ((m & 1) == 0) {
        uint64_t e = 0;
        do {
            m >>= 1;
            ++e;
        } while ((m & 1) == 0);
        /* (1 + 2 + 2^2 + … + 2^e) = 2^(e+1) – 1 */
        sum *= ((uint64_t)1 << (e + 1)) - 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p != 0) continue;
        uint64_t e = 0;
        uint64_t pow = 1;
        while (m % p == 0) {
            m /= p;
            pow *= p;
            ++e;
        }
        /* compute 1 + p + p^2 + … + p^e */
        uint64_t term = 1;
        uint64_t acc = 1;
        for (uint64_t i = 0; i < e; ++i) {
            acc *= p;
            term += acc;
        }
        sum *= term;
    }

    /* remaining prime factor > sqrt(original m) */
    if (m > 1) {
        sum *= (1 + m);
    }

    return sum;
}

#endif
