/* is_prime_trial: whether n is prime, by trial division by a prime table.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_PRIME_TRIAL_H
#define OEISC_UTILS_IS_PRIME_TRIAL_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*--- primality test and prime cache ---------------------------------------*/
static bool is_prime_trial(uint64_t n, const uint64_t *primes, size_t primes_sz) {
    if (n < 2) return false;
    for (size_t i = 0; i < primes_sz; ++i) {
        uint64_t p = primes[i];
        if ((unsigned __int128)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

#endif
