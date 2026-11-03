/* factor_u64: the prime factorization of a 64-bit n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_FACTOR_U64_H
#define OEISC_UTILS_FACTOR_U64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Trial‑division factorisation producing distinct primes
   pr[i] and their multiplicities ex[i].  The caller
   must provide arrays large enough for the factors. */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t i = 0;
    if (n <= 1) { *cnt = 0; return; }
    /* Factor 2. */
    while ((n & 1) == 0) {
        if (i && pr[i - 1] == 2) {
            ++ex[i - 1];
        } else {
            pr[i] = 2;
            ex[i] = 1;
            ++i;
        }
        n >>= 1;
    }
    /* Odd factors. */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            pr[i] = p;
            ex[i] = 0;
            while (n % p == 0) {
                ++ex[i];
                n /= p;
            }
            ++i;
        }
    }
    if (n > 1) {
        pr[i] = n;
        ex[i] = 1;
        ++i;
    }
    *cnt = i;
}

#endif
