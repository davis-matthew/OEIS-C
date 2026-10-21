/*
 * A276770  a(n) is the number of runs of an algorithm. Set b_0 = n, if prime,
 *          stop; else, set c_0 = largest divisor of n (!=n); set b_1 = c_0 +
 *          b_0/c_0. Run with b_1.
 * https://oeis.org/A276770
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations until a prime is reached * by the
 *            following algorithm: * b_0 = n * if b_i is prime, stop *
 *            otherwise let c_i = largest proper divisor of b_i * set b_{i+1}
 *            = c_i + b_i/c_i * continue * a(n) equals the number of times the
 *            step is performed
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Quick primality test for values up to about 1e9 (more than enough here). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Smallest prime factor; returns n itself if n is prime. */
static uint64_t smallest_prime_factor_uint64(uint64_t n)
{
    if (n % 2 == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return d;
    return n;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 5; behaviour below the offset
       is unspecified, however we still compute the value. */
    uint64_t cur = (uint64_t)n;
    seq_t count = 0;

    while (!is_prime_uint64(cur))
    {
        uint64_t spf = smallest_prime_factor_uint64(cur);
        /* For a composite number spf < cur */
        uint64_t d = cur / spf;
        cur = d + spf;
        ++count;
    }
    return count;
}
