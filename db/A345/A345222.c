/*
 * A345222  Number of divisors of n with a prime number of divisors.
 * https://oeis.org/A345222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n such that tau(d) is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* small primality test – works for any value that fits in 64‑bit (our tau values are tiny) */
static inline bool is_prime_small(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* recursive enumeration of all divisor exponent combinations */
static void count_divisors_with_prime_tau(const uint64_t *primes,
                                          const uint64_t *exps,
                                          int num,
                                          int idx,
                                          uint64_t current_tau,
                                          uint64_t *count)
{
    if (idx == num) {
        if (is_prime_small(current_tau))
            ++(*count);
        return;
    }
    for (uint64_t e = 0; e <= exps[idx]; ++e)
        count_divisors_with_prime_tau(primes, exps, num, idx + 1,
                                      current_tau * (e + 1), count);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* safety for indices below offset */

    /* factor n by simple trial division – fast enough for n ≤ 10⁴ */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[32];
    uint64_t exps[32];
    int num = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[num] = p;
            exps[num] = e;
            ++num;
        }
    }
    if (tmp > 1) {
        primes[num] = tmp;
        exps[num] = 1;
        ++num;
    }

    uint64_t count = 0;
    count_divisors_with_prime_tau(primes, exps, num, 0, 1, &count);
    return (seq_t)count;
}
