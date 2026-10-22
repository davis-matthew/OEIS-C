/*
 * A337049  Numbers k such that (k + A000010(k))/2 and (k - A000010(k))/2 are
 *          prime.
 * https://oeis.org/A337049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 815, u32 n <= 351, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k such that (k + φ(k))/2 and (k – φ(k))/2 are prime. */

/* The algorithm factors k on the fly, builds a prime list as needed,
   caches found terms, and supports arbitrary‑precision seq_t via
   the supplied helpers. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* --------------------------------------------------------------------- */
/* Integer square root for 64‑bit unsigned values (floor).  Binary search
   avoids use of sqrt() which conflicts with the GMP headers in the
   arbitrary‑precision build. */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t low = 0, high = x, mid, r = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;  /* avoid infinite loop */
            high = mid - 1;
        }
    }
    return r;
}

/* --------------------------------------------------------------------- */
/* Dynamic prime list: primes[], primes_cnt, primes_cap, prime_limit. */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;
static uint64_t prime_limit = 1;

static void add_prime(uint64_t p)
{
    if (primes_cnt == primes_cap) {
        size_t new_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
        uint64_t *new_arr = (uint64_t *)realloc(primes, new_cap * sizeof(*primes));
        if (!new_arr) abort();
        primes = new_arr;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = p;
    if (p > prime_limit) prime_limit = p;
}

static void init_primes(void)
{
    if (!primes) add_prime(2);
}

static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_limit) return;
    init_primes();
    uint64_t cand = prime_limit + 1;
    for (; cand <= limit; ++cand) {
        bool is_p = true;
        uint64_t r = isqrt_u64(cand);
        for (size_t i = 0; i < primes_cnt && primes[i] <= r; ++i) {
            if (cand % primes[i] == 0) { is_p = false; break; }
        }
        if (is_p) add_prime(cand);
    }
    prime_limit = limit;
}

/* --------------------------------------------------------------------- */
/* Cached sequence values: cache[], cache_len, cache_cap, next_k. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_k = 2;  /* next k to test for the property */

/* --------------------------------------------------------------------- */
/* Test whether a single k satisfies the criterion. */
static bool satisfies(uint64_t k)
{
    if (k & 1) return false;                /* must be even */

    uint64_t orig_k = k;
    uint64_t phi = k;
    bool squarefree = true;

    uint64_t sqrt_k = isqrt_u64(k);
    ensure_primes_upto(sqrt_k);

    for (size_t i = 0; i < primes_cnt && primes[i] <= sqrt_k; ++i) {
        uint64_t p = primes[i];
        if (k % p == 0) {
            int exp = 0;
            while (k % p == 0) { k /= p; ++exp; }
            if (exp > 1) { squarefree = false; break; }
            phi -= phi / p;
            sqrt_k = isqrt_u64(k);
        }
    }
    if (!squarefree) return false;
    if (k > 1) {
        phi -= phi / k;
    }

    seq_t half1 = ((seq_t)orig_k + (seq_t)phi) / 2;
    seq_t half2 = ((seq_t)orig_k - (seq_t)phi) / 2;
    if (half1 < 2 || half2 < 2) return false;
    return is_prime_big(half1) && is_prime_big(half2);
}

/* --------------------------------------------------------------------- */
/* Main exported function.  Returns the nth element of the sequence
   (offset 1).  Subsequent calls are answered from the cache; the cache
   grows as necessary. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined below offset */

    while (cache_len < (size_t)n) {
        if (satisfies(next_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
                if (!new_arr) abort();
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[(size_t)n - 1];
}
