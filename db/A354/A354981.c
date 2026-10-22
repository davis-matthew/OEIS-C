/*
 * A354981  a(n) = 1 if n = 2 * p^k, with p an odd prime and k >= 1, otherwise
 *          0.
 * https://oeis.org/A354981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 2 * p^k with p an odd prime and k >= 1;
 *            otherwise 0
 * Range:     u8 n <= 8500, u16 n <= 8500, u32 n <= 8500, u64 n <= 8500,
 *            u128 n <= 8500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Find the smallest prime factor of an odd number n (n >= 3).
   If n is prime, the function returns n itself. */
static uint64_t _smallest_prime_factor(uint64_t n)
{
    if (n % 3 == 0) return 3;
    uint64_t i = 5;
    while (i * i <= n) {
        if (n % i == 0) return i;
        if (n % (i + 2) == 0) return i + 2;
        i += 6;
    }
    return n; /* n is prime */
}

seq_t a(idx_t n)
{
    /* Only numbers of the form n ≡ 2 (mod 4) can be 2·odd. */
    if ((n & 3) != 2) return (seq_t)0;

    uint64_t m = (uint64_t)(n >> 1);   /* n / 2, m is odd */
    if (m < 3) return (seq_t)0;        /* 2·1 or 2·2 not allowed (p must be odd prime) */

    uint64_t spf = _smallest_prime_factor(m);
    /* spf is odd. Divide out all copies of this prime. */
    while (m % spf == 0) m /= spf;

    /* If the remaining part is 1, m was a pure odd prime power. */
    return (seq_t)(m == 1);
}
