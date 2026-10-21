/*
 * A248516  n^2+1 divided by its largest prime factor.
 * https://oeis.org/A248516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+1) / largest_prime_factor(n^2+1)
 * Range:     u8 n <= 98, u16 n <= 2308, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t largest_prime_factor_uint64(uint64_t n)
{
    if (n <= 1)
        return n;

    uint64_t max_factor = 1;

    /* Remove factors of 2 */
    while ((n & 1) == 0) {
        max_factor = 2;
        n >>= 1;
    }

    /* Test odd divisors */
    for (uint64_t i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            max_factor = i;
            n /= i;
        }
    }

    /* If anything remains, it's a prime factor larger than sqrt(original n) */
    if (n > 1)
        max_factor = n;

    return max_factor;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t m  = nn * nn + 1;          /* compute n^2 + 1 */
    uint64_t lpf = largest_prime_factor_uint64(m);
    return (seq_t)(m / lpf);            /* return (n^2+1) / largest_prime_factor */
}
