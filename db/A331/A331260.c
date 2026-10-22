/*
 * A331260  Denominator of harmonic mean of 3 consecutive primes. Numerators
 *          are A331259.
 * https://oeis.org/A331260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = denominator of the harmonic mean of the * three
 *            consecutive primes p1=prime(n), p2=prime(n+1), p3=prime(n+2). *
 *            The denominator equals (p1*p2 + p1*p3 + p2*p3)/3 if all three
 *            primes * are congruent modulo 3, otherwise it equals (p1*p2 +
 *            p1*p3 + p2*p3)
 * Range:     u8 n <= 3, u16 n <= 34, u32 n <= 4000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset 1: indices start at 1 */
    if (n == 0) return 0;   /* safety, not used by legitimate calls */

    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);
    uint32_t p3 = get_prime(n + 2);

    seq_t sum = (seq_t)p1 * p2 + (seq_t)p1 * p3 + (seq_t)p2 * p3;

    if ((p1 % 3u) == (p2 % 3u) && (p2 % 3u) == (p3 % 3u))
        sum /= 3;

    return sum;
}
