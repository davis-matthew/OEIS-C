/* totient_u32: Euler's totient phi(n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_TOTIENT_U32_H
#define OEISC_UTILS_TOTIENT_U32_H
#include <stdbool.h>
#include <stdint.h>

/* Euler's totient for a 32‑bit unsigned integer. */
static uint32_t totient_u32(uint32_t n)
{
    uint32_t result = n;
    for (uint32_t p = 2; (uint64_t)p * p <= n; ++p)
    {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

#endif
