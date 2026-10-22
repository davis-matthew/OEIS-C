/*
 * A354031  a(n) = 1 if n > 1 and n is a power of a Pythagorean prime (prime
 *          of the form 4m+1), otherwise 0.
 * https://oeis.org/A354031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9880, u16 n <= 9880, u32 n <= 9880, u64 n <= 9880,
 *            u128 n <= 9880, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if n > 1 and n is a power of a Pythagorean prime (prime ≡ 1 mod 4),
   otherwise 0. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Work with n as a 64‑bit unsigned integer; n ≤ 10 000 in tests. */
    uint64_t m = (uint64_t)n;
    uint64_t p = 0;

    /* Find the smallest prime divisor of m. */
    if (m % 2 == 0) {
        p = 2;
    } else {
        uint64_t lim = _floor_isqrt_u64(m);
        for (uint64_t i = 3; i <= lim; i += 2) {
            if (m % i == 0) {
                p = i;
                break;
            }
        }
    }
    /* If m is prime, p = m. */
    if (p == 0)
        p = m;

    /* Remove all powers of p from m. */
    uint64_t t = m;
    while (t % p == 0)
        t /= p;

    /* If after removing p all factors are gone, m was a power of p. */
    if (t != 1)
        return 0;

    /* Check that the prime base is ≡ 1 (mod 4). */
    return (p % 4 == 1) ? 1 : 0;
}
