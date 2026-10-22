/*
 * A324849  Positive integers divisible by none of their prime indices > 1.
 * https://oeis.org/A324849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sequence of positive integers not divisible by any of
 *            their * prime indices > 1. The function generates terms on
 *            demand and memoises * them. It is O(L) where L is the value of
 *            the n‑th term
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/* Prime table for factorisation and index look-up                     */
/* ------------------------------------------------------------------ */
static uint32_t *prime_arr   = NULL;   /* list of primes          */
static uint32_t *prime_index = NULL;   /* 1‑based index of prime  */
static size_t   prime_count = 0;       /* number of stored primes */
static uint64_t prime_limit  = 0;      /* largest integer represented in prime_index */

static void ensure_primes(uint64_t min_limit)
{
    if (min_limit <= prime_limit)
        return;

    /* grow limit (double) but guard against overflow */
    size_t new_limit = (size_t)min_limit * 2;
    if (new_limit < min_limit)
        new_limit = SIZE_MAX / 2;

    bool *is_prime = (bool*)calloc(new_limit + 1, sizeof(bool));
    if (!is_prime)
        exit(1);
    for (size_t i = 2; i <= new_limit; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i * i <= new_limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= new_limit; j += i)
                is_prime[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (is_prime[i])
            ++cnt;

    uint32_t *new_primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!new_primes)
        exit(1);
    for (size_t i = 2, k = 0; i <= new_limit; ++i)
        if (is_prime[i])
            new_primes[k++] = (uint32_t)i;

    /* allocate index array */
    uint32_t *new_index = (uint32_t*)calloc(new_limit + 1, sizeof(uint32_t));
    if (!new_index)
        exit(1);
    for (size_t i = 0; i < cnt; ++i)
        new_index[new_primes[i]] = (uint32_t)(i + 1);   /* 1‑based index */

    /* free old data */
    free(prime_arr);
    free(prime_index);
    free(is_prime);

    prime_arr   = new_primes;
    prime_index = new_index;
    prime_count = cnt;
    prime_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/* Check whether n satisfies the definition of a(n)                    */
/* ------------------------------------------------------------------ */
static bool is_good(uint64_t n)
{
    if (n == 1)
        return true;

    uint64_t tmp = n;

    /* ensure primes up to n (needed for large prime factors) */
    ensure_primes(n);

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_arr[i];
        if ((uint64_t)p * p > tmp)
            break;

        if (tmp % p == 0) {
            uint32_t idx = prime_index[p];
            if (idx > 1 && (n % (uint64_t)idx) == 0)
                return false;
            while (tmp % p == 0)
                tmp /= p;
        }
    }

    if (tmp > 1) {                     /* remaining prime factor > sqrt(n) */
        uint32_t p = (uint32_t)tmp;
        uint32_t idx = prime_index[p];
        if (idx > 1 && (n % (uint64_t)idx) == 0)
            return false;
    }

    return true;
}

/* ------------------------------------------------------------------ */
/* Cache of previously computed valid terms                           */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;
static size_t   cache_len = 0;
static size_t   cache_cap = 0;
static uint64_t tested_up_to = 0;

static void cache_grow(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *newp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!newp && new_cap > 0)
        exit(1);
    seq_cache = newp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Public interface required by the build system                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined, but guard */

    /* extend cache up to n terms                                 */
    while (cache_len < (size_t)n) {
        ++tested_up_to;
        if (is_good(tested_up_to)) {
            cache_grow(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)tested_up_to;
        }
    }

    return seq_cache[n - 1];
}
