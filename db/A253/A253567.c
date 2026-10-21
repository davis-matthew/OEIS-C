/*
 * A253567  Noncomposites together with such composites n = p_i * p_j * p_k *
 *          ... * p_u, p_i <= p_j <= p_k <= ... <= p_u, where there is at
 *          least one such pair of successive prime factors (when sorted into
 *          a nondecreasing order) that the square of the former is larger
 *          than the latter: (p_i)^2 > p_j or (p_j)^...
 * https://oeis.org/A253567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) gives all positive integers that are either 1, a prime,
   or a composite whose prime factorisation contains at least one pair of
   consecutive prime factors p_i <= p_{i+1} (sorted in nondecreasing order)
   with p_i^2 > p_{i+1}.  The numbers are generated in ascending order
   and cached so that a(idx_t n) can be returned in O(1) after the cache
   has been filled up to that index. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_realloc.h"

static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

static uint64_t *prime_list = NULL;
static size_t    prime_cnt  = 0;
static bool      primes_initialized = false;

static void ensure_primes(void)
{
    if (primes_initialized) return;
    size_t cnt = 0;
    /* 100000 is more than enough for the required search range. */
    build_primes_upto(100000, &prime_list, &cnt);
    prime_cnt = cnt;
    primes_initialized = true;
}

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

static bool satisfies(uint64_t n)
{
    if (n == 1) return true;
    if (is_prime(n)) return true;

    ensure_primes();

    uint64_t x = n;
    uint64_t prev = 0;
    bool has_prev = false;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if (p * p > x) break;          /* remaining factors are > sqrt(x) */
        if (x % p == 0)
        {
            uint64_t cnt = 0;
            while (x % p == 0)
            {
                x /= p;
                ++cnt;
            }
            if (cnt >= 2) return true;            /* repeated prime factor */
            if (has_prev && prev * prev > p) return true; /* adjacent pair satisfies */
            prev = p;
            has_prev = true;
        }
    }

    /* any remaining prime factor > sqrt(original n) */
    if (x > 1)
    {
        if (has_prev && prev * prev > x) return true;
    }

    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1; lower indices are unused */

    static uint64_t next_candidate = 1;

    while (cache_size < (size_t)n)
    {
        if (satisfies(next_candidate))
        {
            grow_cache(cache_size + 1);
            cache[cache_size] = (seq_t)next_candidate;
            ++cache_size;
        }
        ++next_candidate;
    }

    return cache[cache_size - 1];
}
