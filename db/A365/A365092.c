/*
 * A365092  Write out the canonical factorization of n and factorize the
 *          exponents in the factorization, the exponents in the
 *          factorizations of the exponents, ... until there are only prime
 *          numbers left. Replace each p in the factorization by (p-1)+1 and
 *          factorize each p-1 by the same process if p-1 > 1. Con...
 * https://oeis.org/A365092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 2, a(p) = a(p-1)+1 for primes p > 2, a(p^e) =
 *            a(p)+a(e), a(m*n) = a(m)+a(n) when gcd(m,n)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Smallest prime factor by trial division (works for n up to 2^64) */
static inline idx_t spf(idx_t n)
{
    if (n <= 3) return n;
    if ((n & 1u) == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return i;
    }
    return n;
}

/* Test whether n is prime (uses spf) */
static inline bool is_prime(idx_t n)
{
    return n > 1 && spf(n) == n;
}

/* Recursive calculation of a(n) */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;   /* a(1) = 0, and a(0) if ever used */
    if (n == 2) return 2;   /* a(2) = 2 */

    if (is_prime(n)) {
        /* a(p) = a(p-1)+1 for primes p > 2 */
        return a(n - 1) + (seq_t)1;
    }

    /* n is composite: break into prime‐power factors */
    seq_t sum = 0;
    idx_t m = n;
    while (m > 1) {
        idx_t p = spf(m);
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        /* a(p^e) = a(p) + a(e) */
        sum += a(p) + a(e);
    }
    return sum;
}
