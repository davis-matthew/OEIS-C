/* next_prime: the smallest prime > n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NEXT_PRIME_H
#define OEISC_UTILS_NEXT_PRIME_H
#include <stdbool.h>
#include <stdint.h>

static uint64_t next_prime(uint64_t p) {
    uint64_t candidate = p + 1;
    
    while (true) {
        bool is_prime = true;
        if (candidate < 2) {
            is_prime = false;
        } else if (candidate == 2) {
            is_prime = true;
        } else if (candidate % 2 == 0) {
            is_prime = false;
        } else {
            for (uint64_t i = 3; i * i <= candidate; i += 2) {
                if (candidate % i == 0) {
                    is_prime = false;
                    break;
                }
            }
        }
        if (is_prime) return candidate;
        candidate++;
    }
}

#endif
