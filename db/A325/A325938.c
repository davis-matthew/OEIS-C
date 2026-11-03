/*
 * A325938  a(n) = omega(n)^tau(n), where omega=A001221 and tau=A000005.
 * https://oeis.org/A325938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = omega(n)^tau(n)
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 209, u64 n <= 839,
 *            u128 n <= 7559, big any n
 * Validated: standard, pass, 84 terms through n = 9358 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* omega(n) is the number of distinct prime factors of n (A001221) */
/* tau(n) is the number of divisors of n (A000005) */

#include <stdint.h>
#include <stdbool.h>

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

/* Count divisors of a 64‑bit integer */
static uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t tau = 1;
    uint64_t exp;

    /* factor 2 */
    exp = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        ++exp;
    }
    if (exp) tau *= (exp + 1);

    /* factor odd primes */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }

    /* remaining prime factor */
    if (n > 1) tau *= 2;

    return tau;
}

/* Multiply-by-self exponentiation for big exponent (seq_t type) */
static seq_t big_pow(seq_t base, seq_t exp) {
    seq_t result = 1;
    while (exp != 0) {
        if ((exp % 2) != 0) {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute omega(n) and tau(n) using available helpers */
    int omega   = count_distinct_primes((uint64_t)n);
    uint64_t tau = divisor_count_u64((uint64_t)n);

    /* Return omega(n) raised to the power tau(n) */
    return big_pow((seq_t)omega, (seq_t)tau);
}
