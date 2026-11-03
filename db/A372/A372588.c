/*
 * A372588  Numbers k > 1 such that (greatest binary index of k) + (greatest
 *          prime index of k) is odd.
 * https://oeis.org/A372588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9461 (2026-11-03)
 * Helpers:   build_primes_upto, gcd_u64, ilog2, is_prime_u64, mem, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A372588:  
   Numbers k > 1 such that the maximum binary index of k plus the
   maximum prime index of k is odd.
   The function builds the terms lazily, caching the results so that
   repeated calls with arbitrary indices are fast. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/gcd_u64.h"
#include "../utils/ilog2.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/mem.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static void ensure_prime_limit(uint64_t limit,
                               uint64_t **primes, size_t *cnt,
                               uint64_t *max_limit)
{
    if (limit <= *max_limit)
        return;
    /* build a new prime list up to 'limit' */
    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);

    /* replace the old list */
    free(*primes);
    *primes = new_primes;
    *cnt    = new_cnt;
    *max_limit = limit;
}

static uint64_t max_prime_index(idx_t n,
                                uint64_t **prime_list, size_t *prime_cnt,
                                uint64_t *prime_limit)
{
    uint64_t max_idx = 0;
    uint64_t m = (uint64_t)n;

    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);   /* trial‑division helper */
        /* make sure our prime table contains all primes up to p */
        ensure_prime_limit(p, prime_list, prime_cnt, prime_limit);

        /* binary search for prime p in the list */
        size_t lo = 0, hi = *prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1U;
            if ((*prime_list)[mid] < p)
                lo = mid + 1;
            else
                hi = mid;
        }
        if (lo < *prime_cnt && (*prime_list)[lo] == p) {
            uint64_t idx = (uint64_t)(lo + 1);  /* indices start at 1 */
            if (idx > max_idx)
                max_idx = idx;
        }
        /* strip all copies of p from m */
        while (m % p == 0)
            m /= p;
    }
    return max_idx;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 2;                /* next integer to test (≥ 2) */

    /* prime table that grows as needed */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static uint64_t prime_limit = 0;

    while (cache_len < n) {
        idx_t k = next_k;

        /* maximum binary index = bit length of k */
        uint64_t bitlen = ilog2(k) + 1ULL;

        /* maximum prime index among prime factors of k */
        uint64_t max_pi = max_prime_index(k,
                                          &prime_list, &prime_cnt,
                                          &prime_limit);

        /* test parity of the sum */
        if (((bitlen + max_pi) & 1ULL) == 1ULL) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }

        ++next_k;
    }

    return cache[n - 1];
}
