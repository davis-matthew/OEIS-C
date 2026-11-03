/* totient_u64: Euler's totient phi(n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_TOTIENT_U64_H
#define OEISC_UTILS_TOTIENT_U64_H
#include <stdint.h>

static uint64_t totient_u64(uint64_t n) {
    uint64_t result = n;
    uint64_t temp = n;
    
    if (temp % 2 == 0) {
        while (temp % 2 == 0)
            temp /= 2;
        result /= 2;
    }
    
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            while (temp % p == 0)
                temp /= p;
            result = result / p * (p - 1);
        }
    }
    
    if (temp > 1) {
        result = result / temp * (temp - 1);
    }
    
    return result;
}

#endif
