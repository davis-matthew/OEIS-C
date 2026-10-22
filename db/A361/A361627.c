/*
 * A361627  Positive integers such that GCD(A007504(n),n) != 1.
 * https://oeis.org/A361627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A361627: the set of positive integers n for which
   GCD( sum of the first n primes , n ) != 1.
   The algorithm builds the sequence incrementally, storing
   only the qualifying n in a cache. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd.h"      /* not used directly – only required if seq_t < 64 bits */
#include "../utils/is_prime.h"

/* ------- static state for the search ------- */
static seq_t            *cache      = NULL;
static size_t            cache_size = 0;
static size_t            cache_cap  = 0;

static uint64_t         *prime_arr   = NULL;
static size_t            primes_cnt = 0;
static size_t            primes_cap = 0;

static uint64_t          candidate   = 2;   /* next number to test for primality */
static idx_t             processed_k = 0;   /* number of primes processed so far */
static uint64_t          sum_of_primes = 0; /* running sum of the primes */

/* ------- helpers ------- */

/* Grow a cache of seq_t elements. */
static void grow_cache(void) {
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) exit(EXIT_FAILURE);
    cache       = tmp;
    cache_cap   = new_cap;
}

/* Grow the primes array. */
static void grow_primes(void) {
    size_t new_cap = (primes_cap == 0) ? 8 : primes_cap * 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
    if (tmp == NULL) exit(EXIT_FAILURE);
    prime_arr   = tmp;
    primes_cap  = new_cap;
}

/* Local unsigned 64‑bit GCD, to avoid overflow with seq_t smaller than 64 bits. */
static uint64_t ugcd_uint64(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return the next prime > 0, storing it in prime_arr. */
static uint64_t next_prime(void) {
    for (;;) {
        if (is_prime(candidate)) {
            if (primes_cnt == primes_cap) grow_primes();
            prime_arr[primes_cnt++] = candidate;
            uint64_t p = candidate;
            ++candidate;
            return p;
        }
        ++candidate;
    }
}

/* ------- sequence function ------- */

seq_t a(idx_t n) {
    /* Build until we have at least n stored terms. */
    while (cache_size < n) {
        uint64_t p = next_prime();
        sum_of_primes += p;
        ++processed_k;                  /* processed_k == the current k */

        /* Check GCD condition. */
        if (ugcd_uint64(sum_of_primes, (uint64_t)processed_k) != 1ULL) {
            if (cache_size == cache_cap) grow_cache();
            cache[cache_size++] = (seq_t)processed_k;
        }
    }
    return cache[n - 1];
}
