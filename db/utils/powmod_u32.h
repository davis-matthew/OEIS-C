/* powmod_u32: b^e mod m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POWMOD_U32_H
#define OEISC_UTILS_POWMOD_U32_H
#include <stdbool.h>
#include <stdint.h>

/* Modular exponentiation: base^exp mod mod (mod ≤ PRIME_LIMIT) */
static uint32_t powmod_u32(uint32_t base, uint32_t exp, uint32_t mod)
{
    uint32_t result = 1u % mod;
    base %= mod;
    while (exp) {
        if (exp & 1u) result = (uint32_t)((uint64_t)result * base % mod);
        base = (uint32_t)((uint64_t)base * base % mod);
        exp >>= 1u;
    }
    return result;
}

#endif
