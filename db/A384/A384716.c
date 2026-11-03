/*
 * A384716  The totient of the product of unitary divisors of n.
 * https://oeis.org/A384716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(2^(omega(n)-1)-1) * phi(n) (for n > 1) a(1) = 1 where
 *            omega(n) is the number of distinct prime factors of n
 * Range:     u8 n <= 25, u16 n <= 29, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
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
    if (n == 1)            /* a(1) = 1 */
        return (seq_t)1;

    /* Number of distinct prime factors of n */
    unsigned omega = (unsigned)count_distinct_primes((uint64_t)n);

    /* exp = 2^(omega-1) – 1 */
    size_t shift = omega - 1;
    seq_t exp = ((seq_t)1 << shift) - 1;

    /* n^exp */
    seq_t pow_part = big_pow((seq_t)n, exp);

    /* φ(n) */
    seq_t phi_part = phi((idx_t)n);

    return pow_part * phi_part;
}
