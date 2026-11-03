/* mobius_u64: the Moebius function mu(n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MOBIUS_U64_H
#define OEISC_UTILS_MOBIUS_U64_H
#include <stdint.h>

static int mobius_u64(uint64_t n) {
    if (n == 1) return 1;
    
    int count = 0;
    
    if (n % 2 == 0) {
        count++;
        n /= 2;
        if (n % 2 == 0) return 0;
    }
    
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            count++;
            n /= p;
            if (n % p == 0) return 0;
        }
    }
    
    if (n > 1) count++;
    return (count & 1) ? -1 : 1;
}

#endif
