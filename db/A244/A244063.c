/*
 * A244063  Number of prime factors (with multiplicity) of the number of
 *          distinct prime factors of n; a(n) = Omega(omega(n)).
 * https://oeis.org/A244063
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Omega(omega(n))
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 102 terms through n = 4999 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of prime factors (with multiplicity) of the number of distinct prime factors of n. */

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

/* Count prime factors of m (with multiplicity) */
static seq_t count_prime_factors(seq_t m) {
    seq_t cnt = 0;
    for (idx_t p = 2; (idx_t)p * (idx_t)p <= m; ++p) {
        while (m % p == 0) {
            ++cnt;
            m /= p;
        }
    }
    if (m > 1) ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* behaviour below the offset is irrelevant */
    uint64_t u = (uint64_t)n;
    uint64_t omega = count_distinct_primes(u); /* omega(n) */
    seq_t omega_seq = (seq_t)omega; /* cast to seq_t for the next routine */
    return count_prime_factors(omega_seq); /* Omega(omega(n)) */
}
