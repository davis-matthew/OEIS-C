/*
 * A287620  a(n) = product, with multiplicity, of the prime numbers appearing
 *          at leaf positions in the prime tower factorization of n.
 * https://oeis.org/A287620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product with multiplicity of the prime numbers appearing
 *            at leaf positions in the prime tower factorization of n.
 *            Recurrence: a(p) = p (prime p), a(p^k) = a(k) for k>1,
 *            multiplicative over distinct primes
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

static seq_t calc_a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (is_prime(n)) return (seq_t)n;

    seq_t result = 1;
    idx_t m = n;

    /* handle factor 2 */
    if ((m & 1) == 0) {
        idx_t exp = 0;
        while ((m & 1) == 0) { m >>= 1; exp++; }
        result *= (exp == 1) ? (seq_t)2 : calc_a(exp);
    }

    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) { m /= p; exp++; }
            result *= (exp == 1) ? (seq_t)p : calc_a(exp);
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) result *= (seq_t)m;

    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, handled for completeness */
    return calc_a(n);
}
