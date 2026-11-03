/*
 * A323135  a(n) is the least number of iterations that n requires to reach a
 *          power of a prime under the map x -> A070229(x), or -1 if we never
 *          reach a power of a prime.
 * https://oeis.org/A323135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least number of iterations of the map x -> x + gpf(x)
 *            until a prime power is reached; returns -1 (max unsigned) if
 *            never
 * Range:     u8 n <= 525, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9479 (2026-11-01)
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"

static bool is_prime_power(uint64_t v)
{
    if (v <= 1) return true;                     /* 1 is considered a prime power */
    uint64_t p = largest_prime_factor(v);
    while (v % p == 0) v /= p;
    return v == 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;                /* out-of-range safety (offset 1) */
    uint64_t x = (uint64_t)n;                    /* start value */
    idx_t steps = 0;
    while (1) {
        if (is_prime_power(x))
            return (seq_t)steps;
        uint64_t g = largest_prime_factor(x);
        x += g;
        ++steps;
        /* safety guard: stop after a reasonable number of steps */
        if (steps > 1000000)
            return (seq_t)-1;
    }
}
