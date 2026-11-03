/*
 * A348988  Numerator of A332994(n) / sigma(n).
 * https://oeis.org/A348988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A332994(n) / gcd(sigma(n), A332994(n)) where A332994(n) =
 *            sum of the chain { n, ⌊n/p⌋, ⌊⌊n/p⌋/p'\⌋, ... } with p, p', ...
 *            being the successive largest prime factors of each term, until 1
 *            is reached
 * Range:     u8 n <= 191, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 104 terms through n = 9530 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

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

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1, but guard anyway */

    /* Compute A332994(n) : sum of the divisor chain. */
    seq_t sum = 0;
    seq_t cur = (seq_t)n;
    uint64_t m   = n;

    while (m > 1) {
        sum += cur;                       /* add current term to the sum */
        uint64_t p = largest_prime_divisor(m);
        m   /= p;                         /* remove one instance of its largest prime divisor */
        cur  /= p;                         /* same for the value stored in cur */
    }
    /* add the final term '1' of the chain */
    sum += (seq_t)1;

    /* Compute sigma(n) and reduce the fraction. */
    seq_t sigma = sigma_of(n);           /* sum of all positive divisors of n */
    seq_t g = gcd(sum, sigma);
    return sum / g;
}
