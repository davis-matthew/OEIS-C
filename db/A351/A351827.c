/*
 * A351827  Sum of the numbers <= n that are either prime, a divisor of n, or
 *          both.
 * https://oeis.org/A351827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - sopf(n) + Σ_{p ≤ n, p prime} p where sigma(n)
 *            is the sum of all positive divisors of n and sopf(n) is the sum
 *            of the distinct prime divisors of n
 * Range:     u8 n <= 39, u16 n <= 899, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1) /* sequence is indexed starting at 1 */
        return (seq_t)0;

    /* ----- Prime table (cached up to the largest n seen so far) ----- */
    static uint64_t *prime_tbl = NULL;
    static size_t   prime_cnt  = 0;
    static idx_t    max_n      = 0;

    if (n > max_n) {
        /* Build sieve up to n */
        size_t sieve_size = (size_t)(n + 1);
        uint8_t *is_prime = (uint8_t *)malloc(sieve_size);
        if (!is_prime)
            return (seq_t)0;           /* memory failure guarded */
        memset(is_prime, 1, sieve_size);
        if (sieve_size > 0) is_prime[0] = 0;
        if (sieve_size > 1) is_prime[1] = 0;

        for (idx_t p = 2; p <= n / p; ++p) {
            if (is_prime[p]) {
                for (idx_t j = p * p; j <= n; j += p)
                    is_prime[j] = 0;
            }
        }

        /* Count primes and store them */
        prime_cnt = 0;
        for (idx_t i = 2; i <= n; ++i)
            if (is_prime[i])
                ++prime_cnt;

        prime_tbl = (uint64_t *)realloc(prime_tbl,
                                       prime_cnt * sizeof(uint64_t));
        size_t pos = 0;
        for (idx_t i = 2; i <= n; ++i)
            if (is_prime[i])
                prime_tbl[pos++] = (uint64_t)i;

        free(is_prime);
        max_n = n;
    }

    /* ----- Sum of primes ≤ n ----- */
    seq_t sum_primes = (seq_t)0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];
        if (p > n)
            break;
        sum_primes += (seq_t)p;
    }

    /* ----- Sigma(n) and sum of distinct prime divisors (sopf) ----- */
    idx_t m = n;
    seq_t sigma    = (seq_t)1;
    seq_t sopf_sum = (seq_t)0;

    for (size_t i = 0; i < prime_cnt && prime_tbl[i] <= m; ++i) {
        uint64_t p = prime_tbl[i];
        if (m % p == 0) {
            sopf_sum += (seq_t)p;          /* distinct prime divisor */

            /* Compute 1 + p + p^2 + … + p^e for exponent e of p */
            seq_t term   = (seq_t)1;
            seq_t power  = (seq_t)1;
            while (m % p == 0) {
                m /= p;
                power *= (seq_t)p;
                term += power;
            }
            sigma *= term;
        }
    }

    /* Factor remaining large prime, if any */
    if (m > 1) {
        sopf_sum += (seq_t)m;
        sigma *= ((seq_t)1 + (seq_t)m);
    }

    /* ----- Result ----- */
    return sum_primes + sigma - sopf_sum;
}
