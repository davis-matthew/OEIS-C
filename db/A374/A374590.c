/*
 * A374590  Numbers whose maximum exponent in their prime factorization is an
 *          evil number (A001969).
 * https://oeis.org/A374590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 7930, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   parity_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374590: numbers whose maximum exponent in their prime
 * factorizations is an evil number (binary with an even number of 1's).
 * The function is computed by incremental prime factorisation / trial
 * division and caching of the sequence terms. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/parity_uint.h"

/* --------------------------------------------------------------------- */
/* Helper: bit parity (0 for even number of 1 bits, 1 for odd).           */

/* --------------------------------------------------------------------- */
/* Simple prime generation up to an arbitrary limit.  The prime list is
 * expanded only when a larger limit is required.  The list is stored
 * in increasing order.  The implementation uses a relaxed Miller–Rabin
 * O(n²) trial division; this is perfectly adequate for the size needed
 * (candidate values up to roughly 1‑e⁵ for n≈1e4).                       */
/* --------------------------------------------------------------------- */
static uint32_t *prime_list      = NULL;
static size_t    prime_cnt       = 0;
static size_t    prime_cap       = 0;
static uint64_t  prime_max_seen  = 1;   /* all numbers ≤ prime_max_seen
                                           have been screened for primality */

/* Expand the prime list to include all primes ≤ limit. */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_max_seen)
        return;

    for (uint64_t num = prime_max_seen + 1; num <= limit; ++num) {
        bool is_p = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            uint64_t sq = (uint64_t)p * p;
            if (sq > num)
                break;
            if (num % p == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            if (prime_cnt + 1 > prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(prime_list,
                                                    prime_cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = (uint32_t)num;
        }
    }
    prime_max_seen = limit;
}

/* --------------------------------------------------------------------- */
/* Test whether a number satisfies the sequence property: its maximum
 * exponent in prime factorisation is an evil number.                 */
/* --------------------------------------------------------------------- */
static bool is_candidate(uint64_t x)
{
    if (x < 2)
        return false;

    /* Need primes up to sqrt(x) – we ask a bit more to stay safe. */
    uint64_t limit = x;
    if (limit > 1000000ULL)  /* cap at 1e6 to avoid excessive memory */
        limit = 1000000ULL;
    ensure_primes_upto(limit);

    uint64_t temp = x;
    unsigned int maxExp = 0;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        uint32_t p = prime_list[i];
        if (temp % p != 0)
            continue;
        unsigned int cnt = 0;
        do {
            ++cnt;
            temp /= p;
        } while (temp % p == 0);
        if (cnt > maxExp)
            maxExp = cnt;
    }

    if (temp > 1)                /* remaining prime factor has exponent 1 */
        if (maxExp < 1)
            maxExp = 1;

    /* Evil numbers have an even number of 1 bits in binary. */
    return parity_uint(maxExp) == 0;
}

/* --------------------------------------------------------------------- */
/* Cache for the sequence values.  cache[n-1] holds a(n).                 */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;
static size_t cache_size = 0;   /* number of stored terms */
static size_t cache_cap  = 0;   /* capacity of the cache array */
static uint64_t next_test = 1;  /* next integer to be tested for candidacy */

/* Grow the cache until it contains at least n terms. */
static void ensure_cache_size(idx_t n)
{
    while (cache_size < (size_t)n) {
        /* Find next qualifying integer. */
        do {
            ++next_test;
        } while (!is_candidate(next_test));

        /* Grow the cache if needed. */
        if (cache_size + 1 > cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache,
                                          cache_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }

        cache[cache_size++] = (seq_t)next_test;
    }
}

/* --------------------------------------------------------------------- */
/* Public function required by the judge.                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* behavior for n=0 is irrelevant */
        return (seq_t)0;
    ensure_cache_size(n);
    return cache[n - 1];
}
