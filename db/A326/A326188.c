/*
 * A326188  a(n) = A001065(n) - A003557(n), where A001065(n) = the sum of
 *          proper divisors of n, and A003557(n) = n divided by its largest
 *          squarefree divisor.
 * https://oeis.org/A326188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A001065(n) - A003557(n) (sum of proper divisors of n)
 *            minus (n divided by its largest squarefree divisor)
 * Range:     i8 n <= 12, i16 n <= 682, i32 n <= 9361, i64 n <= 9361,
 *            i128 n <= 9361, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* Precomputed prime table (sieve up to 100000) */
    static uint64_t primes[100001];
    static size_t prime_cnt = 0;
    static unsigned char composite[100001];

    if (prime_cnt == 0) {
        const size_t LIMIT = 100000;
        /* Sieve of Eratosthenes */
        for (size_t i = 0; i <= LIMIT; ++i) composite[i] = 0;
        for (size_t i = 2; i * i <= LIMIT; ++i) {
            if (!composite[i]) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    composite[j] = 1;
            }
        }
        for (size_t i = 2; i <= LIMIT; ++i) {
            if (!composite[i]) primes[prime_cnt++] = i;
        }
    }

    uint64_t temp = (uint64_t)n;
    seq_t sigma = 1;      /* σ(n) */
    seq_t rad   = 1;      /* product of distinct primes of n */

    /* Factor n using the precomputed primes. */
    for (size_t pi = 0; pi < prime_cnt && primes[pi] * primes[pi] <= temp; ++pi) {
        uint64_t p = primes[pi];
        if (temp % p == 0) {
            unsigned int exp = 0;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }
            /* Compute (p^(exp+1)-1)/(p-1) */
            seq_t pe1 = 1;
            for (unsigned int j = 0; j < exp + 1; ++j)
                pe1 *= (seq_t)p;
            seq_t term = (pe1 - 1) / ((seq_t)p - 1);
            sigma *= term;
            rad   *= (seq_t)p;
        }
    }

    /* Handle remaining prime factor > sqrt(original n). */
    if (temp > 1) {
        uint64_t p = temp;
        /* exponent = 1 */
        seq_t pe1 = 1;
        for (unsigned int j = 0; j < 2; ++j)   /* p^2 */
            pe1 *= (seq_t)p;
        seq_t term = (pe1 - 1) / ((seq_t)p - 1);   /* = p + 1 */
        sigma *= term;
        rad   *= (seq_t)p;
    }

    seq_t sum_prop  = sigma - (seq_t)n;   /* sum of proper divisors */
    seq_t n_div_rad = (seq_t)n / rad;    /* n / rad(n) */

    return sum_prop - n_div_rad;
}
