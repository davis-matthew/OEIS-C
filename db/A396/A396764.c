/*
 * A396764  Nonprimes whose average of aliquot divisors and average of
 *          divisors are both integers.
 * https://oeis.org/A396764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2647, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: composite numbers whose average of aliquot divisors and
   average of all divisors are integers */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Prime sieve limit – sufficient to factor all numbers in the sequence
   up to about 10¹². */
static const uint32_t PRIME_SIEVE_LIMIT = 5000000;

static bool primes_initialized = false;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

/* Create a list of primes up to PRIME_SIEVE_LIMIT. */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    bool *is_prime = (bool *)malloc((PRIME_SIEVE_LIMIT + 1) * sizeof(bool));
    for (uint32_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_prime[j] = false;

    /* Count primes. */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i])
            ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    prime_count = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i])
            prime_list[prime_count++] = i;

    free(is_prime);
    primes_initialized = true;
}

/* Compute σ(n) (sum of divisors), τ(n) (number of divisors) and
   determine whether n is composite. */
static void sigma_tau_composite(uint64_t n, seq_t *sigma, seq_t *tau,
                                bool *is_composite)
{
    *sigma = 1;
    *tau   = 1;
    uint64_t temp = n;
    size_t distinct = 0;
    bool saw_exp_gt1 = false;

    for (size_t i = 0; i < prime_count &&
                       (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        uint32_t p = prime_list[i];
        if (temp % p == 0) {
            distinct++;
            int exp = 0;
            seq_t power = 1;
            seq_t sum   = 1;      /* sum of geometric series for this p */
            while (temp % p == 0) {
                temp /= p;
                exp++;
                power = power * (seq_t)p;
                sum   = sum + power;
            }
            if (exp > 1)
                saw_exp_gt1 = true;
            *sigma = (*sigma) * sum;
            *tau   = (*tau) * (exp + 1);
        }
    }

    if (temp > 1) {              /* remaining prime factor > sqrt(n) */
        distinct++;
        *sigma = (*sigma) * (seq_t)(temp + 1);
        *tau   = (*tau) * 2;
    }

    *is_composite = (distinct > 1) || saw_exp_gt1;
}

/* Cache for already found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 2;

/* Append a found term to the cache. */
static void cache_append(seq_t val)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len >= cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Generate terms up to at least nth term. */
static void compute_until(idx_t n)
{
    init_primes();

    while (cache_len < n) {
        seq_t sigma, tau;
        bool composite;
        sigma_tau_composite(next_candidate, &sigma, &tau, &composite);

        if (composite) {
            seq_t proper_sum = sigma - (seq_t)next_candidate;
            seq_t proper_cnt = tau - (seq_t)1;
            /* proper_cnt is always >0 for composite numbers */
            if ((proper_sum % proper_cnt) == 0 && (sigma % tau) == 0)
                cache_append((seq_t)next_candidate);
        }
        ++next_candidate;
    }
}

/* External function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, but return 0 */
    compute_until(n);
    return cache[n - 1];
}
