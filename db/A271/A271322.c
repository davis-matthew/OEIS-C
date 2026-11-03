/*
 * A271322  Largest prime factor of the n-th n-gonal number (A060354).
 * https://oeis.org/A271322
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime divisor of n*(n^2 - 3n + 4)/2, offset 2
 * Range:     u8 n <= 24, u16 n <= 364, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 917 (2026-11-01)
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

seq_t a(idx_t n) {
    /* The n‑th n‑gonal number is
       P(n) = n * (n^2 - 3n + 4) / 2                     */
    uint64_t nn = (uint64_t)n;
    uint64_t P = nn * (nn * nn - 3 * nn + 4) / 2;

    /* Largest prime divisor of P, using the provided helper */
    uint64_t lpf = largest_prime_divisor(P);

    return (seq_t)lpf;
}
