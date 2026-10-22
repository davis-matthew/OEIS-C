/*
 * A375032  The maximum odd exponent in the prime factorization of n, or 0 if
 *          no such exponent exists.
 * https://oeis.org/A375032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum odd exponent of any prime in the factorization of
 *            n; a(n) = 0 if n is a perfect square (no odd exponents)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Parameter n is an unsigned 64‑bit integer. */
    if (n == 0)         /* undefined index; safe return */
        return (seq_t)0;

    uint64_t x = (uint64_t)n;
    uint64_t max_odd = 0;
    uint64_t exp;

    /* Handle factor 2 separately. */
    exp = 0;
    while ((x & 1ULL) == 0) {      /* while x is even */
        x >>= 1;                   /* divide by 2 */
        ++exp;
    }
    if (exp & 1ULL)                /* exponent is odd */
        max_odd = exp;

    /* Handle odd prime factors by trial division. */
    for (uint64_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) {
            exp = 0;
            while (x % d == 0) {
                x /= d;
                ++exp;
            }
            if ((exp & 1ULL) && exp > max_odd)
                max_odd = exp;
        }
    }

    /* If anything remains, it is a prime factor with exponent 1. */
    if (x > 1 && max_odd < 1)
        max_odd = 1;

    return (seq_t)max_odd;
}
