/*
 * A397148  Numbers k such that k + omega(k) is prime.
 * https://oeis.org/A397148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A397148  
   Sequence: numbers k such that k + ω(k) is prime (ω = number of distinct prime factors). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"          /* bool is_prime_big(seq_t) */

/* Integer square root by binary search (uses 128‑bit intermediate to avoid overflow). */
static idx_t int_sqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)  lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;
}

/* ----------  Prime sieve & list  ---------- */
static bool *prime_sieve = NULL;    /* prime_sieve[i] == true  ⇔  i is prime */
static idx_t  prime_limit = 0;      /* maximum i for which sieve exists  */
static idx_t *primes = NULL;        /* array of primes ≤ prime_limit */
static size_t primes_len = 0;       /* number of primes stored         */
static size_t primes_cap = 0;       /* capacity of primes array         */

static void extend_sieve(idx_t new_limit)
{
    bool *sieve = (bool *)malloc((new_limit + 1) * sizeof(bool));
    for (idx_t i = 0; i <= new_limit; ++i) sieve[i] = true;
    if (new_limit >= 0) sieve[0] = false;
    if (new_limit >= 1) sieve[1] = false;

    idx_t sq = int_sqrt_idx(new_limit);
    for (idx_t i = 2; i <= sq; ++i) if (sieve[i]) {
        for (idx_t j = i * i; j <= new_limit; j += i) sieve[j] = false;
    }

    /* rebuild primes array */
    free(primes);
    primes = NULL;
    primes_len = 0;
    primes_cap = 0;
    for (idx_t i = 2; i <= new_limit; ++i) if (sieve[i]) {
        if (primes_len == primes_cap) {
            primes_cap = primes_cap ? primes_cap * 2 : 16;
            primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
        }
        primes[primes_len++] = i;
    }

    free(prime_sieve);
    prime_sieve = sieve;
    prime_limit  = new_limit;
}

static void ensure_primes_up_to(idx_t n)
{
    if (n > prime_limit) {
        idx_t new_lim = n + n / 10 + 100;
        extend_sieve(new_lim);
    }
}

/* Count distinct prime factors of x using the global prime list. */
static idx_t count_distinct_prime_factors(idx_t x)
{
    if (x <= 1) return 0;
    ensure_primes_up_to(int_sqrt_idx(x) + 1);

    idx_t cnt = 0;
    idx_t rem = x;
    for (size_t i = 0; i < primes_len && (idx_t)primes[i]*primes[i] <= rem; ++i) {
        idx_t p = primes[i];
        if (rem % p == 0) {
            ++cnt;
            while (rem % p == 0) rem /= p;
        }
    }
    if (rem > 1) ++cnt;
    return cnt;
}

/* ----------  Sequence cache  ---------- */
static seq_t *cache     = NULL;   /* cache[n-1] = a(n) */
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  last_k    = 0;      /* largest k examined so far */

static void ensure_cache_capacity(size_t needed)
{
    if (cache_len >= needed) return;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* ----------  Public function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined offset, but safe */

    /* Generate terms until at least n terms have been cached. */
    while (cache_len < n) {
        idx_t k = last_k + 1;
        while (true) {
            idx_t omega = count_distinct_prime_factors(k);
            seq_t cand = (seq_t)k + (seq_t)omega;
            if (is_prime_big(cand)) {
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len++] = (seq_t)k;
                last_k = k;
                break;   /* move to next term */
            }
            ++k;
        }
    }
    return cache[n - 1];
}
