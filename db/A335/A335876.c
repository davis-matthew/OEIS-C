/*
 * A335876  a(1) = 2, and for n > 1, a(n) = n + (n/p), where p is largest
 *          prime dividing n, A006530(n).
 * https://oeis.org/A335876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + (n / p), where p is the largest prime divisor of n
 *            and a(1) = 2
 * Range:     u8 n <= 191, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 99 terms through n = 9656 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n == 1) return 2;

    /* Largest prime divisor of n (n is uint64_t). */
    uint64_t p = largest_prime_divisor((uint64_t)n);

    /* Update result in seq_t.  The division is performed in uint64_t,
       then promoted to seq_t. */
    return (seq_t)n + (seq_t)(n / p);
}
