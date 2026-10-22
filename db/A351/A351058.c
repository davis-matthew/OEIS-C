/*
 * A351058  Number of numbers <= n that are either nonprime divisors of n or
 *          primes not dividing n.
 * https://oeis.org/A351058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π(n) + τ(n) - 2·ω(n) where π(n) = number of primes ≤ n,
 *            τ(n) = number of divisors of n, ω(n) = number of distinct prime
 *            factors of n
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Small prime test for 64‑bit integers (sufficient for n ≤ 10^4). */
static bool _is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0 || x % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= x; i += 6)
        if (x % i == 0 || x % (i + 2) == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Count primes ≤ n (π(n)). */
    seq_t pi = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (_is_prime_uint64((uint64_t)i))
            ++pi;

    /* Factorize n to compute τ(n) and ω(n). */
    uint64_t tmp = (uint64_t)n;
    seq_t tau = 1;
    size_t omega = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (!_is_prime_uint64(p))
            continue;
        if (tmp % p == 0) {
            ++omega;
            unsigned int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            tau *= (seq_t)(exp + 1);
        }
    }
    if (tmp > 1) {         /* remaining prime factor */
        ++omega;
        tau *= 2;           /* exponent 1 -> exp+1 = 2 */
    }

    /* a(n) = π(n) + τ(n) - 2·ω(n). */
    return pi + tau - (seq_t)(2u * (uint64_t)omega);
}
