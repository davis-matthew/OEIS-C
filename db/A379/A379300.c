/*
 * A379300  Number of prime indices of n that are composite.
 * https://oeis.org/A379300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ e_p * [index_of_prime(p) is composite] where n = ∏
 *            p^e_p, and index_of_prime(p) is the rank of the prime p
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static tables for primes and prime indices */
static uint32_t *primes = NULL;                /* list of primes up to current_limit */
static size_t    primes_cnt = 0;              /* number of primes stored */
static uint32_t *prime_index_of = NULL;        /* prime_index_of[p] = rank of prime p (1‑based) */
static bool     *prime_isprime_idx = NULL;    /* prime_isprime_idx[i] = true iff i is prime (index i) */
static idx_t    current_limit = 0;             /* largest number already sieved */

/* Ensure that all tables contain data up to at least `limit`. */
static void ensure_limit(idx_t limit)
{
    if (limit <= current_limit)
        return;

    size_t sz = (size_t)limit + 1;

    /* allocate a sieve array */
    bool *sieve = (bool*)calloc(sz, sizeof(bool));
    if (!sieve)
        abort();

    /* temporarily count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            ++cnt;
            if (i <= limit / i)
                for (size_t j = i * i; j <= limit; j += i)
                    sieve[j] = true;
        }
    }

    /* build primes array and prime_index_of table */
    uint32_t *new_primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!new_primes)
        abort();
    uint32_t *new_index_of = (uint32_t*)calloc(sz, sizeof(uint32_t));
    if (!new_index_of)
        abort();

    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            new_primes[pos] = (uint32_t)i;
            new_index_of[i] = (uint32_t)(pos + 1); /* rank is 1‑based */
            ++pos;
        }
    }

    /* free sieve array */
    free(sieve);

    /* build prime_isprime_idx table up to primes_cnt (inclusive) */
    size_t idx_lim = cnt;
    bool *new_idx_prime = (bool*)calloc(idx_lim + 1, sizeof(bool));
    if (!new_idx_prime)
        abort();
    for (size_t i = 2; i <= idx_lim; ++i)
        new_idx_prime[i] = true;
    new_idx_prime[0] = new_idx_prime[1] = false;
    for (size_t i = 2; i * i <= idx_lim; ++i) {
        if (new_idx_prime[i]) {
            for (size_t j = i * i; j <= idx_lim; j += i)
                new_idx_prime[j] = false;
        }
    }

    /* replace old structures */
    free(primes);
    free(prime_index_of);
    free(prime_isprime_idx);
    primes = new_primes;
    prime_index_of = new_index_of;
    prime_isprime_idx = new_idx_prime;
    primes_cnt = cnt;
    current_limit = limit;
}

/* Count the number of prime indices of n that are composite. */
seq_t a(idx_t n)
{
    if (n < 2)               /* n = 1 has no prime factors */
        return (seq_t)0;

    ensure_limit(n);          /* ensure tables up to n */

    uint64_t tmp = (uint64_t)n;
    seq_t count = 0;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint32_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            uint32_t idx = prime_index_of[p];
            if (idx >= 2 && !prime_isprime_idx[idx])
                count += (seq_t)exp;
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {
        uint32_t idx = prime_index_of[(uint32_t)tmp];
        if (idx >= 2 && !prime_isprime_idx[idx])
            count += (seq_t)1;
    }

    return count;
}
