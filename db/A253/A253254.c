/*
 * A253254  Largest prime factor of the n-th 11-gonal number.
 * https://oeis.org/A253254
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest_prime_divisor((9*n*n - 7*n)/2)
 * Range:     u8 n <= 29, u16 n <= 7295, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9331 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* compute the n-th 11-gonal number ((9*n^2 - 7*n)/2) */
    uint64_t un = (uint64_t)n;
    __uint128_t val = (__uint128_t)9 * un * un - (__uint128_t)7 * un;
    val /= 2;                     /* now val == number */

    /* factorise the 64‑bit value */
    uint64_t p = (uint64_t)val;   /* safe for n up to ~10^6 (see comments below) */
    return (seq_t)largest_prime_divisor(p);
}
