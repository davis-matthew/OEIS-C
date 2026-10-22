/*
 * A305974  a(1) = 1; for n > 1, if n = p^k for some prime p and exponent k >=
 *          1, then a(n) = -k, otherwise a(n) = 1+A085970(n).
 * https://oeis.org/A305974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1; for n>1, if n=p^k then a(n)=-k; otherwise a(n)=n-(#prime
 *            powers ≤ n)
 * Range:     i8 n <= 183, i16 n <= 9865, i32 n <= 9865, i64 n <= 9865,
 *            i128 n <= 9865, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


/* static state for caching */
static idx_t max_cache_n = 0;        /* largest n processed so far   */
static uint8_t *is_pp     = NULL;     /* true iff index is a prime power */
static idx_t *pp_prefix   = NULL;     /* prefix sums of is_pp           */
static uint64_t *primes   = NULL;     /* list of primes ≤ max_cache_n   */
static size_t   primes_cnt = 0;        /* number of primes in the list   */
static size_t   primes_cap = 0;        /* allocated capacity of primes   */

/* finds the smallest prime factor of n using the cached primes list; */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n % 2 == 0)
        return 2;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return p;
    }
    /* if none found, n itself is prime */
    return n;
}

/* recompute all tables up to n (inclusive); */
static void recompute_up_to(idx_t n)
{
    if (n <= max_cache_n)
        return;  /* already up to date */

    /* allocate space for sieve of primes up to n */
    size_t sz = (size_t)n + 1;
    uint8_t *is_prime = (uint8_t *)malloc(sz);
    if (!is_prime)
        abort();
    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = 1;
    if (sz > 0)
        is_prime[0] = is_prime[1] = 0;

    /* sieve */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = 0;
        }
    }

    /* collect primes into the static list */
    if (primes_cap < (size_t)n) {
        uint64_t *tmp = (uint64_t *)realloc(primes, n * sizeof(uint64_t));
        if (!tmp)
            abort();
        primes   = tmp;
        primes_cap = (size_t)n;
    }
    primes_cnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            if (primes_cnt >= primes_cap) {
                size_t new_cap = primes_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp)
                    abort();
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = i;
        }
    }

    /* allocate / resize tables for prime powers and prefix sums */
    if (!is_pp || !pp_prefix) {
        is_pp   = (uint8_t *)malloc(sz);
        pp_prefix = (idx_t *)malloc(sz * sizeof(idx_t));
        if (!is_pp || !pp_prefix)
            abort();
    } else {
        is_pp   = (uint8_t *)realloc(is_pp, sz);
        pp_prefix = (idx_t *)realloc(pp_prefix, sz * sizeof(idx_t));
        if (!is_pp || !pp_prefix)
            abort();
    }

    for (size_t i = 0; i < sz; ++i)
        is_pp[i] = 0;

    /* mark all prime powers ≤ n */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        uint64_t val = p;
        while (val <= n) {
            is_pp[val] = 1;
            if (val > n / p)
                break;
            val *= p;
        }
    }

    /* build prefix sums of is_pp */
    pp_prefix[0] = 0;
    if (sz > 1)
        pp_prefix[1] = 0;
    for (idx_t i = 2; i < (idx_t)sz; ++i) {
        pp_prefix[i] = pp_prefix[i - 1] + (is_pp[i] ? 1 : 0);
    }

    free(is_prime);
    max_cache_n = n;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    recompute_up_to(n);

    if (is_pp[n]) {                   /* n is a prime power */
        uint64_t p = smallest_prime_factor(n);
        uint64_t tmp = n, k = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++k;
        }
        return -(seq_t)k;
    } else {                           /* otherwise */
        return (seq_t)n - (seq_t)pp_prefix[n];
    }
}
