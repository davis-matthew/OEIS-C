/*
 * A390330  Numbers k such that the number of distinct prime divisors of
 *          binomial(2k, k) is odd.
 * https://oeis.org/A390330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth number k such that the number of distinct prime
 *            divisors of binomial(2k, k) is odd
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   carry_exists
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../utils/carry_exists.h"

/* ------------------------------------------------------------------------- */
/* Sieve --------------------------------------------------------------- */
static bool *is_prime   = NULL;   /* primality flags 0..sieve_limit */
static size_t sieve_limit = 0;    /* current maximum of sieve */
static uint64_t *primes    = NULL;/* list of primes <= sieve_limit*/
static size_t   primes_cnt = 0;   /* number of primes            */

static void extend_sieve(size_t new_limit)
{
    if (new_limit <= sieve_limit) return;
    sieve_limit = new_limit;

    is_prime = (bool*)realloc(is_prime, (sieve_limit + 1) * sizeof(bool));
    memset(is_prime, 1, (sieve_limit + 1) * sizeof(bool));
    if (sieve_limit >= 0) is_prime[0] = false;
    if (sieve_limit >= 1) is_prime[1] = false;

    for (size_t p = 2; (unsigned long long)p * p <= sieve_limit; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= sieve_limit; m += p)
                is_prime[m] = false;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= sieve_limit; ++i)
        if (is_prime[i]) ++cnt;

    free(primes);
    primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= sieve_limit; ++i)
        if (is_prime[i]) primes[idx++] = i;
    primes_cnt = cnt;
}

static inline size_t count_primes_le(uint64_t n)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= n) lo = mid + 1;
        else hi = mid;
    }
    return lo;          /* number of primes <= n */
}

/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* Parity test ------------------------------------------------------------ */
static inline bool has_odd_distinct_prime_divisors(uint64_t k)
{
    size_t cnt = count_primes_le(2 * k) - count_primes_le(k);
    for (size_t i = 0; i < primes_cnt && primes[i] <= k; ++i)
        if (carry_exists(primes[i], k)) ++cnt;
    return (cnt & 1u) != 0;
}

/* ------------------------------------------------------------------------- */
/* Cache of sequence values ---------------------------------------------- */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_sz  = 0;
static uint64_t last_k  = 0;

seq_t a(idx_t n)
{
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_sz = 0;
        last_k = 0;
        extend_sieve(1024);
    }

    while (cache_sz <= (size_t)n) {
        ++last_k;
        unsigned __int128 two_last = (unsigned __int128)last_k * 2;
        if (two_last > (unsigned __int128)sieve_limit)
            extend_sieve((size_t)(two_last * 2));

        if (has_odd_distinct_prime_divisors(last_k)) {
            if (cache_sz == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_sz++] = (seq_t)last_k;
        }
    }
    return cache[(size_t)n];
}
