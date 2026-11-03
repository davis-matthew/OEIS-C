/* nth_prime_u64: the n-th prime, prime(1) = 2.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NTH_PRIME_U64_H
#define OEISC_UTILS_NTH_PRIME_U64_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t nth_prime_u64(uint64_t n) {
    uint64_t lim = 64;
    for (;;) {
        unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
        uint64_t cnt = 0, i, j;
        for (i = 2; i <= lim; i++) {
            if (!comp[i]) {
                cnt++;
                if (cnt == n) { free(comp); return i; }
                for (j = i * i; j <= lim; j += i) comp[j] = 1;
            }
        }
        free(comp);
        lim *= 2;
    }
}

#endif
