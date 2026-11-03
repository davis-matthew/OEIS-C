/* is_prime_i64: whether n is prime.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_PRIME_I64_H
#define OEISC_UTILS_IS_PRIME_I64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Small helper: primality test for 64‑bit values (S(k) never exceeds this) */
static bool is_prime_i64(long long v)
{
    if (v <= 1) return false;
    if (v % 2 == 0) return v == 2;
    for (long long d = 3; d * d <= v; d += 2)
        if (v % d == 0) return false;
    return true;
}

#endif
