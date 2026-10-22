/*
 * A332993  a(1) = 1, for n > 1, a(n) = n + a(A032742(n)).
 * https://oeis.org/A332993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = n + a(A032742(n)) (largest proper
 *            divisor)
 * Range:     u8 n <= 135, u16 n <= 9518, u32 n <= 9518, u64 n <= 9518,
 *            u128 n <= 9518, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Find the smallest prime factor of n; if n is prime return n */
static idx_t _smallest_prime_factor(idx_t n)
{
    if (n <= 1) return n;
    if (n % 2 == 0) return 2;
    for (idx_t p = 3; p * p <= n; p += 2)
        if (n % p == 0) return p;
    return n;                     /* n is prime */
}

/* Largest proper divisor of n (n >= 2) is n / (smallest prime factor) */
static idx_t _largest_proper_divisor(idx_t n)
{
    if (n <= 1) return 1;
    idx_t spf = _smallest_prime_factor(n);
    return (spf == n) ? 1 : n / spf;
}

/* Recursive definition of the sequence */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    idx_t lp = _largest_proper_divisor(n);   /* largest proper divisor */
    return (seq_t)n + a(lp);
}
