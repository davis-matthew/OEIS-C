/*
 * A338281  a(n) is the sum of n and the largest proper divisor of n.
 * https://oeis.org/A338281
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + A032742(n) (largest proper divisor of n)
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9661 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                   /* undefined for n < 2, but harmless */

    uint64_t spf = lpf(n);          /* smallest prime factor of n */
    uint64_t largest_proper_divisor = n / spf;

    return (seq_t)n + (seq_t)largest_proper_divisor;
}
