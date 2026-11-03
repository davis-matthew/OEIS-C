/*
 * A319710  a(n) = 1 if n is divisible by the square of its smallest prime
 *          factor, 0 otherwise.
 * https://oeis.org/A319710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is divisible by the square of its smallest prime
 *            factor, 0 otherwise
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 8652 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n <= 1) return (seq_t)0;
    idx_t spf = lpf(n);               /* smallest prime factor of n */
    if (spf == 0) return (seq_t)0;    /* safety check; should not occur */
    if (n % spf != 0) return (seq_t)0;
    n /= spf;
    if (n % spf != 0) return (seq_t)0;
    return (seq_t)1;
}
