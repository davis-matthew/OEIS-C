/*
 * A253254  Largest prime factor of the n-th 11-gonal number.
 * https://oeis.org/A253254
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest_prime_divisor((9*n*n - 7*n)/2)
 * Range:     u8 n <= 29, u16 n <= 7295, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_divisor.h"

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
