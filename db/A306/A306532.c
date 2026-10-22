/*
 * A306532  Composite numbers k such that the sum of their aliquot parts
 *          divides k+1.
 * https://oeis.org/A306532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: returns the n-th composite k such that the sum of its proper divisors 
 * (the aliquot sum) divides k+1.
 * Method: brute force search with memoisation and a small prime sieve.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

#define MAX_PRIME_SIEVE 200000   /* should cover all k up to the required range */

/* global prime sieve */
static uint64_t *prime_array = NULL;
static size_t prime_cnt = 0;
static bool primes_init = false;

static void init_primes(void)
{
    if (!primes_init) {
        build_primes_upto((idx_t)MAX_PRIME_SIEVE, &prime_array, &prime_cnt);
        primes_init = true;
    }
}

/* test primality using the small sieve */
static bool is_prime_num(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_array[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* sum of proper divisors (aliquot sum) */
static seq_t sum_proper_divisors(uint64_t n)
{
    seq_t sigma = 1;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_array[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                ++exp;
                tmp /= p;
            }
            /* term = (p^(exp+1) - 1) / (p - 1) */
            seq_t power = 1;
            for (uint64_t k = 0; k < exp + 1; ++k) power *= (seq_t)p;
            seq_t num = power - (seq_t)1;
            seq_t denom = (seq_t)(p - 1);
            seq_t term = num / denom;
            sigma *= term;
        }
    }
    if (tmp > 1) {
        sigma *= ((seq_t)tmp + (seq_t)1);
    }
    return sigma - (seq_t)n;   /* proper divisor sum */
}

/* memoised cache of found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t current_k = 3;          /* last checked integer (init to 3) */
static idx_t current_count = 0;         /* terms found so far */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety for out‑of‑range call */

    init_primes();

    while (current_count < n) {
        ++current_k;
        if (is_prime_num(current_k)) continue;   /* skip primes */

        seq_t s = sum_proper_divisors(current_k);
        if (s == (seq_t)0) continue;            /* never happens for composites */

        seq_t k1 = (seq_t)current_k + (seq_t)1;
        if (k1 % s == (seq_t)0) {               /* condition satisfied */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
                seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();       /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)current_k;
            ++current_count;
        }
    }

    return cache[n - 1];
}
