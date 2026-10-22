/*
 * A341660  Primes p such that p^2 - 1 has fewer than 32 divisors.
 * https://oeis.org/A341660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) returns the nth prime p such that tau(p^2-1) < 32. The
 *            function searches primes incrementally, computes the divisor
 *            count of p^2-1 by trial division (sufficient for the small p
 *            values that satisfy the condition), and returns the result. If n
 *            exceeds the number of such primes, 0 is returned
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static unsigned int divisor_count_u64(uint64_t n)
{
    unsigned int tau = 1;
    unsigned int exp;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            exp = 0;
            do {
                n /= p;
                ++exp;
            } while (n % p == 0);
            tau *= (exp + 1);
        }
    }
    if (n > 1) tau *= 2;
    return tau;
}

static bool is_small_prime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
        i += 6;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1, undefined for n=0 */
    uint64_t count = 0;
    uint64_t candidate = 2;
    /* All primes satisfying the condition are <= 73, so a generous
       upper bound suffices. */
    const uint64_t UPPER = 1000;
    while (candidate <= UPPER) {
        if (is_small_prime(candidate)) {
            uint64_t val = candidate * candidate - 1;
            unsigned int tau = divisor_count_u64(val);
            if (tau < 32) {
                ++count;
                if (count == n) {
                    return (seq_t)candidate;
                }
            }
        }
        ++candidate;
    }
    return 0;   /* n too large, no such prime */
}
