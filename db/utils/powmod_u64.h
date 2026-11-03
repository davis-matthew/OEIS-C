/* powmod_u64: b^e mod m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POWMOD_U64_H
#define OEISC_UTILS_POWMOD_U64_H
#include <stdbool.h>
#include <stdint.h>

/* fast modular exponentiation for 64-bit values */
static uint64_t powmod_u64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

#endif
