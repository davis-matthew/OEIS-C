/*
 * A349217  a(n) = Sum_{d|n} n^c(d), where c is the prime characteristic
 *          (A010051).
 * https://oeis.org/A349217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*omega(n) + tau(n) (omega = distinct prime factors,
 *            tau = divisor count)
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers used by both C11 and C++17. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- Prime handling ---- */
static idx_t   prime_limit = 1;      /* largest number up to which primes are known */
static uint64_t *primes = NULL;      /* array of primes */
static size_t   primes_cnt = 0;      /* number of primes stored */

/* Generate primes up to at least `limit` using a simple sieve. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;                       /* nothing to do */

    size_t n = (size_t)limit;

    /* Mark all numbers as prime initially. */
    bool *is_prime = (bool*)malloc(n + 1);
    if (!is_prime)
        return;                       /* allocation failed – leave state unchanged */
    for (size_t i = 0; i <= n; ++i)
        is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    for (size_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }

    /* Count new primes and extend the primes array. */
    size_t new_count = 0;
    for (size_t i = prime_limit + 1; i <= n; ++i)
        if (is_prime[i])
            ++new_count;

    uint64_t *new_primes = (uint64_t*)realloc(primes,
                     (primes_cnt + new_count) * sizeof(uint64_t));
    if (!new_primes) {
        free(is_prime);
        return;                       /* allocation failed – leave state unchanged */
    }
    primes = new_primes;

    /* Append new primes. */
    for (size_t i = prime_limit + 1, j = primes_cnt; i <= n; ++i) {
        if (is_prime[i])
            primes[j++] = (uint64_t)i;
    }
    primes_cnt += new_count;
    prime_limit = limit;

    free(is_prime);
}

/* ---- Cache for computed values ---- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Make sure the cache can hold at least indices up to `n`. */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    size_t new_cap = n < 16 ? 16 : n * 2;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        return;                       /* allocation failed – keep old cache */

    /* Initialize new slots to zero. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_cap = new_cap;
}

/* ---- Main function ---- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n < 1, defensive guard */

    /* Use cache if possible. */
    size_t idx = (size_t)n - 1;
    ensure_cache(n);
    if (cache[idx] != 0)
        return cache[idx];

    /* Ensure we have enough primes to factor n. */
    ensure_primes(n);

    /* Factor n to obtain ω(n) and τ(n). */
    idx_t rem = n;
    seq_t omega = 0;   /* distinct prime factors */
    seq_t tau   = 1;   /* divisor count */

    for (size_t i = 0; i < primes_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= rem; ++i) {
        idx_t p = (idx_t)primes[i];
        if (rem % p == 0) {
            ++omega;                /* new distinct prime factor */
            idx_t exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            tau *= (seq_t)(exp + 1);
        }
    }

    if (rem > 1) {                   /* remaining prime factor */
        ++omega;
        tau *= 2;
    }

    /* a(n) = (n-1)*omega + tau */
    seq_t result = ((seq_t)(n - 1)) * omega + tau;

    cache[idx] = result;
    return result;
}
