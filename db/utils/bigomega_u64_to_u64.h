/* bigomega_u64_to_u64: the number of prime factors of n, with multiplicity.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_BIGOMEGA_U64_TO_U64_H
#define OEISC_UTILS_BIGOMEGA_U64_TO_U64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Count Ω(m) for a 64‑bit unsigned integer.  It uses a simple
   trial‑division factorisation; sufficient for the size of
   numbers appearing in this sequence. */
static uint64_t bigomega_u64_to_u64(uint64_t x)
{
    uint64_t cnt = 0;
    while (x > 1ULL) {
        if ((x & 1ULL) == 0ULL) {
            /* factor 2 repeatedly */
            while ((x & 1ULL) == 0ULL) {
                x >>= 1;
                ++cnt;
            }
        } else {
            /* odd primes starting at 3 */
            uint64_t p = 3ULL;
            while (p * p <= x) {
                if (x % p == 0ULL) {
                    while (x % p == 0ULL) {
                        x /= p;
                        ++cnt;
                    }
                }
                p += 2ULL;
            }
            /* x itself is prime */
            if (x > 1ULL) {
                ++cnt;
                x = 1ULL;
            }
        }
    }
    return cnt;
}

#endif
