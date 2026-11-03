/*
 * A384763  Product of the Euler totients of the unitary divisors of n.
 * https://oeis.org/A384763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{d|n, gcd(d, n/d)=1} phi(d) Equivalent: a(n) =
 *            phi(n)^(2^(omega(n)-1)) (omega = number of distinct prime
 *            factors) with a(1)=1
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute Euler's totient φ of an odd 64‑bit or larger number m.
   The algorithm uses trial division up to sqrt(m). */
static seq_t calc_phi(seq_t m)
{
    seq_t result = m;
    seq_t p = 2;

    /* Handle factor 2 separately to skip it later (m is odd, but keep robust) */
    if (m % 2 == 0) {
        while (m % 2 == 0)
            m /= 2;
        result -= result / 2;
    }

    /* Trial division by odd primes */
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            while (m % p == 0)
                m /= p;
            result -= result / p;
        }
    }

    /* If m > 1, it is a prime factor larger than sqrt(original m) */
    if (m > 1)
        result -= result / m;

    return result;
}

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

/* fast exponentiation by squaring: base^exp in seq_t */
static seq_t seq_pow(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* Euler's totient of n. */
    seq_t phi_n = calc_phi((seq_t)n);

    /* Number of distinct prime factors of n. */
    int omega = count_distinct_primes((uint64_t)n);
    if (omega <= 0) return 1;           /* Should not happen except n==1. */

    /* exponent = 2^(omega-1). */
    idx_t exp = 1ULL << (omega - 1);

    /* Raise phi(n) to the required power. */
    return seq_pow(phi_n, exp);
}
