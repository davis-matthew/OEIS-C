/*
 * A385122  a(n) = d(phi(n)) - phi(d(n)) where d(n) = A000005(n) is the number
 *          of divisors and phi(n) = A000010(n) is the Euler totient function.
 * https://oeis.org/A385122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = d(phi(n)) - phi(d(n)) where d(n) = A000005(n) is the
 *            number of divisors and phi(n) = A000010(n) is Euler's totient
 *            function
 * Range:     i8 n <= 349, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 8808 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    /* phi(n) */
    seq_t phi_n = phi(n);

    /* number of divisors of phi(n) */
    seq_t d_phi_n = (seq_t)divisor_count_u64((uint64_t)phi_n);

    /* d(n) */
    seq_t d_n = (seq_t)divisor_count_u64((uint64_t)n);

    /* phi(d(n)) */
    seq_t phi_d_n = phi((idx_t)d_n);

    return d_phi_n - phi_d_n;
}
