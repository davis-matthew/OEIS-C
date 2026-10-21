/*
 * A295431  a(n) = (12*n)!*n! / ((6*n)!*(4*n)!*(3*n)!).
 * https://oeis.org/A295431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n)! * n! / ((6*n)! * (4*n)! * (3*n)!)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the binomial ratio using prime exponents (Legendre form). */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute the exponent of prime p in m! */
static unsigned long long exp_in_fact(idx_t m, uint64_t p)
{
    unsigned long long e = 0;
    for (idx_t div = p; div <= m; div *= p)
        e += m / div;
    return e;
}

/* Sieve of Eratosthenes, returns all primes <= limit. */
static void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    size_t sieve_size = (size_t)limit + 1;
    bool *is_prime = (bool*)calloc(sieve_size, 1);
    if (!is_prime) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i < sieve_size; ++i)
        is_prime[i] = true;
    for (size_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (size_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *pr = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!pr) {
        free(is_prime);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) pr[pos++] = (uint64_t)i;

    free(is_prime);
    *primes_out = pr;
    *cnt_out = cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t limit = 12 * n;
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    seq_t result = 1;
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        long long e = (long long)exp_in_fact(12 * n, p)
                    + (long long)exp_in_fact(n, p)
                    - (long long)exp_in_fact(6 * n, p)
                    - (long long)exp_in_fact(4 * n, p)
                    - (long long)exp_in_fact(3 * n, p);
        if (e <= 0) continue;
        unsigned long long mults = (unsigned long long)e;
        for (unsigned long long k = 0; k < mults; ++k)
            result *= (seq_t)p;
    }

    free(primes);
    return result;
}
