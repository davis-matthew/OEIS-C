/*
 * A390558  Numbers whose square is an exponential abundant number.
 * https://oeis.org/A390558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 3485, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th term of OEIS sequence A390558.
   The sequence consists of the positive integers k for which
   σ_E(k²) > 2·k², where σ_E is the sum of exponential divisors
   of a number.  The values are discovered by an incremental
   search and cached for future calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Static data used for memoisation ------------------------------------- */
static seq_t *cache      = NULL;   /* cached sequence values          */
static size_t cache_len  = 0;      /* number of cached values          */
static size_t cache_cap  = 0;      /* current cache capacity           */

static uint64_t *prime_list = NULL; /* array of primes used for factoring */
static size_t prime_cnt    = 0;      /* number of primes                  */

static uint64_t current_k  = 1;     /* last integer examined            */

/* --------------------------------------------------------------------- */
/* Initialise the prime table up to a fixed upper bound.                 */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;
    size_t ncnt;
    /* 1 500 000 is more than enough for the first 10 000 terms.        */
    build_primes_upto(1500000, &prime_list, &ncnt);
    prime_cnt = ncnt;
}

/* --------------------------------------------------------------------- */
/* Compute p^e where p is unsigned and e is unsigned.                    */
static seq_t pow_uint_seq(uint64_t p, uint64_t e)
{
    seq_t r = 1;
    for (uint64_t i = 0; i < e; ++i)
        r *= (seq_t)p;
    return r;
}

/* --------------------------------------------------------------------- */
/* Return true iff k satisfies σ_E(k²) > 2·k².                           */
static bool qualifies(uint64_t k)
{
    uint64_t n = k;
    seq_t sigma = 1;

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= n; ++i) {
        uint64_t p   = prime_list[i];
        uint64_t e   = 0;
        if (n % p != 0)
            continue;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        uint64_t a   = 2 * e;   /* exponent of p in k²          */
        seq_t sum    = 0;
        for (uint64_t d = 1; d <= a; ++d)
            if (a % d == 0)
                sum += pow_uint_seq(p, d);
        sigma *= sum;
    }

    /* If remaining part is >1 it is a prime factor of k.             */
    if (n > 1) {
        uint64_t p   = n;
        uint64_t e   = 1;
        uint64_t a   = 2 * e;
        seq_t sum    = 0;
        for (uint64_t d = 1; d <= a; ++d)
            if (a % d == 0)
                sum += pow_uint_seq(p, d);
        sigma *= sum;
    }

    seq_t k_sq        = (seq_t)k * (seq_t)k;
    seq_t two_k_sq    = k_sq + k_sq;
    return sigma > two_k_sq;
}

/* --------------------------------------------------------------------- */
/* Ensure the cache has room for at least 'needed' elements.             */
static void ensure_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;

    if (cache == NULL)
        cache = seq_alloc(new_cap);
    else
        cache = seq_realloc(cache, cache_cap, new_cap);

    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Generate sequence terms until we have cached at least `up_to`.        */
static void generate_terms(size_t up_to)
{
    init_primes();

    while (cache_len < up_to) {
        ++current_k;
        if (qualifies(current_k)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)current_k;
        }
    }
}

/* --------------------------------------------------------------------- */
/* The function required by the evaluation framework.                    */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* defined only for n >= 1 */

    generate_terms((size_t)n);
    return cache[(size_t)n - 1];
}
