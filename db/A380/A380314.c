/*
 * A380314  Numerator of sum of reciprocals of all prime divisors of all
 *          positive integers <= n.
 * https://oeis.org/A380314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Σ_{p ≤ n} ⌊n/p⌋ / p, where the sum runs over
 *            all primes p ≤ n. The function returns only the numerator of the
 *            reduced fraction
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 29, u64 n <= 54, u128 n <= 102,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Compute the numerator of the sum of reciprocals of all prime divisors
   of all positive integers ≤ n.  The result is stored in type seq_t.
   The function works for any build, where seq_t may be any unsigned
   integer type (8, 16, 32, 64, 128 bits or arbitrary precision).  */
seq_t a(idx_t n)
{
    /* For n < 2 there are no primes ≤ n, so the sum is 0. */
    if (n < 2) return 0;

    /* ---------- 1. Sieve primes up to n ---------- */
    size_t limit = (size_t)(n + 1);                /* array size */
    uint8_t *is_prime = (uint8_t *)malloc(limit);  /* 1 byte per number  */
    if (!is_prime) return 0;                      /* allocation failed */

    /* Initialize all entries to true (prime). */
    for (size_t i = 0; i < limit; ++i) is_prime[i] = 1;
    is_prime[0] = 0;
    if (limit > 1) is_prime[1] = 0;

    /* Classic Eratosthenes sieve */
    for (size_t i = 2; i * i <= (size_t)n; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)n; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes to allocate array. */
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)n; ++i)
        if (is_prime[i]) ++cnt;

    /* Array of primes. */
    idx_t *primes = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!primes) { free(is_prime); return 0; }

    size_t pos = 0;
    for (size_t i = 2; i <= (size_t)n; ++i)
        if (is_prime[i]) primes[pos++] = (idx_t)i;

    free(is_prime);

    /* ---------- 2. Accumulate the fraction ---------- */
    seq_t num = 0;   /* numerator of the current sum */
    seq_t den = 1;   /* denominator of the current sum */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t k   = (seq_t)(n / (idx_t)p);   /* floor(n/p) */

        /* add k/p to num/den:
           new_num/new_den = num/den + k/p     */
        seq_t g    = gcd(den, p);             /* gcd(den, p)      */
        seq_t lcm  = (den / g) * p;           /* lcm(den, p)      */
        seq_t part1 = num * (p / g);
        seq_t part2 = k   * (den / g);
        seq_t new_num = part1 + part2;
        seq_t new_den = lcm;

        /* reduce the fraction again */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    free(primes);
    return num;          /* numerator of the reduced fraction */
}
