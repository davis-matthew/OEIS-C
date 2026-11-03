/*
 * A285183  Nearest integer to n*omega(n)/phi(n).
 * https://oeis.org/A285183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to n*omega(n)/phi(n)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 8903 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

seq_t a(idx_t n)
{
    /* Compute the number of distinct prime factors ω(n) */
    int omega = count_distinct_primes((uint64_t)n);

    /* Compute Euler's totient φ(n) */
    seq_t phi_val = phi(n);

    /* n*omega is small for the sequence range, but compute in seq_t */
    seq_t numerator = (seq_t)n * (seq_t)omega;

    /* Integer division: q = floor(numerator/phi_val) */
    seq_t q = numerator / phi_val;
    seq_t r = numerator % phi_val;

    /* Round to nearest integer: if 2*r ≥ phi_val, round up */
    if ((r << 1) >= phi_val)
        ++q;

    return q;
}
