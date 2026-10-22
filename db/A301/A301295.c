/*
 * A301295  Smallest distance from n to a prime power (as defined in A000961).
 * https://oeis.org/A301295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = min{|m–n| : m is a prime power (pᵏ with p prime, k≥0)}
 * Range:     u8 n <= 9307, u16 n <= 9307, u32 n <= 9307, u64 n <= 9307,
 *            u128 n <= 9307, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether x is a prime power (1 or pᵏ with p prime, k≥1). */
static bool is_prime_power_uint64(uint64_t x)
{
    if (x == 1) return true;          /* 1 = p⁰ */
    if (x % 2 == 0) {                  /* even number – only possible if x is a power of 2 */
        while (x % 2 == 0) x /= 2;
        return (x == 1);
    }
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            return (x == 1);
        }
    }
    /* Remaining x is 1 or a prime (exponent 1). */
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset, harmless */

    uint64_t nval = (uint64_t)n;
    uint64_t d = 0;

    for (;;) {
        /* Check lower side (n – d) if it stays ≥1. */
        if (d < nval) {
            uint64_t lower = nval - d;
            if (is_prime_power_uint64(lower))
                return (seq_t)d;
        }

        /* Check upper side (n + d), but avoid overflow. */
        if (d > ((uint64_t)-1) - nval)
            break;                       /* cannot search further */
        uint64_t upper = nval + d;
        if (is_prime_power_uint64(upper))
            return (seq_t)d;

        d++;                               /* increase distance */
    }

    /* Should never reach here for positive n, but return a large value as a fall‑back. */
    return (seq_t)-1;
}
