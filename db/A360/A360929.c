/*
 * A360929  Odd numbers which cannot be expressed as p + q*(q+1) where p and q
 *          are primes.
 * https://oeis.org/A360929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th odd integer that cannot be expressed as p +
 *            q*(q+1) where p and q are primes
 * Range:     u8 n <= 17, u16 n <= 137, u32 n <= 167, u64 n <= 167,
 *            u128 n <= 167, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"

#ifndef LIMIT
#  define LIMIT (2000000ull)       /* pre‑compute primes up to this value */
#endif

/* Prime sieve and list of primes up to LIMIT */
static uint8_t   *prime_sieve   = NULL;   /* 0/1 flags for primality */
static uint32_t  *prime_list    = NULL;   /* list of primes */
static size_t    prime_cnt     = 0;
static bool      primes_init   = false;

/* Build the sieve and prime list */
static void init_primes(void)
{
    if (primes_init) return;

    size_t n = LIMIT;
    prime_sieve = (uint8_t *)calloc(n + 1, sizeof(uint8_t));
    if (!prime_sieve) abort();
    for (size_t i = 0; i <= n; ++i) prime_sieve[i] = 1u;
    prime_sieve[0] = prime_sieve[1] = 0u;

    for (size_t i = 2; i * i <= n; ++i) {
        if (prime_sieve[i]) {
            for (size_t j = i * i; j <= n; j += i) prime_sieve[j] = 0u;
        }
    }

    /* Count primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= n; ++i) if (prime_sieve[i]) ++prime_cnt;

    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= n; ++i) {
        if (prime_sieve[i]) prime_list[idx++] = (uint32_t)i;
    }

    primes_init = true;
}

/* Test whether x can be written as p + q*(q+1) with primes p,q */
static bool representable(seq_t x)
{
    /* Iterate over all primes q until q*(q+1) >= x */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t q = prime_list[i];
        seq_t base = (seq_t)q * (seq_t)(q + 1);
        if (base >= x) break;
        seq_t p = x - base;
        if (p <= (seq_t)2) continue;        /* p must be an odd prime */
        if ((p & 1u) == 0u) continue;        /* p must be odd */
        if (p <= (seq_t)LIMIT) {
            /* Small p: look up the sieve */
            if (prime_sieve[(size_t)p]) return true;
        } else {
            /* Large p: use provided big‑prime tester */
            if (is_prime_big(p)) return true;
        }
    }
    return false;
}

/* Cache of computed terms of the sequence */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_size = 0;
static seq_t next_odd    = 1;        /* next odd number to test */

/* Grow cache array as needed */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* OEIS sequence A360929 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset is 1; this value is irrelevant */

    init_primes();

    /* Generate terms until we have n of them */
    while (cache_size < (size_t)n) {
        seq_t x = next_odd;
        if (!representable(x)) {
            if (cache_size >= cache_cap) grow_cache();
            cache[cache_size++] = x;
        }
        next_odd += 2;
        /* The pre‑computed sieve covers all values in the sequence
           (the largest term is well below LIMIT), so we never run
           out of bounds.  */
    }
    return cache[n - 1];
}
