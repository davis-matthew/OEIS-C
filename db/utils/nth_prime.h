/* nth_prime: the n-th prime, prime(1) = 2.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NTH_PRIME_H
#define OEISC_UTILS_NTH_PRIME_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t nth_prime(idx_t n) {
    uint64_t c = 0, x = 1;
    while (c < n) {
        x++;
        bool pr = true;
        for (uint64_t d = 2; d * d <= x; d++)
            if (x % d == 0) { pr = false; break; }
        if (pr) c++;
    }
    return x;
}

#endif
