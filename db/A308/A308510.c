/*
 * A308510  Prime(k) such that the multiplicative order of prime(k) (mod
 *          prime(k+1)) = prime(k+1)-1.
 * https://oeis.org/A308510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 2376, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p such that p is a primitive root modulo the next prime
   (prime(k+1)).  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mod_pow.h"

/* Static state for the sequence */
static bool          initialized      = false;
static uint64_t      *prime_list       = NULL;
static size_t        prime_len        = 0;
static size_t        prime_cap        = 0;
static seq_t         *cache            = NULL;
static size_t        cache_len        = 0;
static size_t        cache_cap        = 0;
static uint64_t      current_prev     = 0;

/* Ensure the prime list buffer is large enough. */
static void
ensure_prime_cap(size_t needed)
{
    if (prime_cap < needed) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        uint64_t *p = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        prime_list = p;
        prime_cap  = new_cap;
    }
}

/* Ensure the cache buffer is large enough. */
static void
ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = p;
        cache_cap = new_cap;
    }
}

/* Test whether p is a primitive root modulo q (both primes). */
static bool
is_primitive_root(uint64_t p, uint64_t q)
{
    uint64_t ph   = q - 1;
    uint64_t temp = ph;
    uint64_t factors[20];
    size_t   fcnt  = 0;

    /* Distinct prime factorisation of q-1. */
    for (uint64_t i = 2; i * i <= temp; ++i) {
        if (temp % i == 0) {
            factors[fcnt++] = i;
            while (temp % i == 0) temp /= i;
        }
    }
    if (temp > 1) factors[fcnt++] = temp;

    /* Primitive root test using mod_pow. */
    for (size_t i = 0; i < fcnt; ++i) {
        seq_t r = mod_pow((seq_t)p, ph / factors[i], (seq_t)q);
        if (r == (seq_t)1) return false;
    }
    return true;
}

/* Return the next prime after 'start', extending prime_list. */
static uint64_t
next_prime_from(uint64_t start)
{
    uint64_t candidate = start + 1;
    if (candidate <= 2) candidate = 2;
    if (candidate > 2 && (candidate & 1ULL) == 0) candidate++;

    for (;;) {
        bool prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            ensure_prime_cap(prime_len + 1);
            prime_list[prime_len++] = candidate;
            return candidate;
        }
        candidate += 2; /* Skip even numbers. */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (!initialized) {
        /* Prime list starts with 2. */
        ensure_prime_cap(1);
        prime_list[prime_len++] = 2;
        current_prev = 2;

        /* Cache starts empty. */
        ensure_cache_cap(1);
        initialized = true;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        uint64_t next_q = next_prime_from(current_prev);
        if (is_primitive_root(current_prev, next_q)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)current_prev;
        }
        current_prev = next_q;
    }

    return cache[(size_t)n - 1];
}
