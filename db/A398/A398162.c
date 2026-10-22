/*
 * A398162  Exponentially odd numbers k such that the prime factorization
 *          exponents of k+1 are either 1 or even.
 * https://oeis.org/A398162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A398162 by brute‑force search. */
/* The sequence consists of the exponentially odd numbers k
   for which k+1 has all prime exponents equal to 1 or even. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Small prime sieve used for factorisation.  The bound is generous
   enough for n up to ~10^5 (the 10 000‑th term is < 2×10^4). */
#define MAX_PRIME_LIMIT 200000
static uint32_t prime_list[MAX_PRIME_LIMIT + 1];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void) {
    if (primes_ready) return;
    bool sieve[MAX_PRIME_LIMIT + 1];
    for (size_t i = 0; i <= MAX_PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i) sieve[j] = false;
        }
    }
    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (sieve[i]) prime_list[prime_cnt++] = (uint32_t)i;
    }
    primes_ready = true;
}

/* Check that all prime exponents in n are odd. */
static bool is_exp_odd(uint64_t n) {
    if (n == 1) return true; /* vacuous */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        uint64_t pp = (uint64_t)p * p;
        if (pp > n) break;
        if (n % p == 0) {
            unsigned int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if ((exp & 1) == 0) return false; /* even exponent */
        }
    }
    /* Remaining prime factor (if any) has exponent 1 which is odd */
    return true;
}

/* Check that all prime exponents in n are either 1 or even. */
static bool cond_k_plus_one(uint64_t n) {
    if (n == 1) return true; /* no prime factors */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        uint64_t pp = (uint64_t)p * p;
        if (pp > n) break;
        if (n % p == 0) {
            unsigned int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if (!(exp == 1 || (exp & 1) == 0)) return false;
        }
    }
    /* Remaining prime factor (if any) has exponent 1 */
    return true;
}

/* Cache of generated terms.  The cache contains at least n terms
   when a(n) is requested. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

static void grow_cache(void) {
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_mem) abort();
    cache = new_mem;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* guard against 0 index */
    init_primes();
    /* Generate terms until we have at least n of them. */
    if (cache_size < (size_t)n) {
        uint64_t candidate = cache_size ? (uint64_t)cache[cache_size - 1] + 1 : 1;
        while (cache_size < (size_t)n) {
            if (is_exp_odd(candidate) && cond_k_plus_one(candidate + 1)) {
                if (cache_size == cache_cap) grow_cache();
                cache[cache_size++] = (seq_t)candidate;
            }
            ++candidate;
        }
    }
    return cache[n - 1];
}
