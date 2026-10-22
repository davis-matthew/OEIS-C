/*
 * A319246  Sum of prime indices of the n-th squarefree number.
 * https://oeis.org/A319246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sum of prime indices of the n-th squarefree number */

/* We generate square‑free integers one after another and keep a
 * cache of the sum of the 1‑based indices of their distinct prime
 * divisors.  The cache is rebuilt only up to the largest n requested.
 * Prime indices are obtained from a list of primes that is enlarged on
 * demand using build_primes_upto.
 *
 * All arithmetic is performed in seq_t.  For speed the cache is a
 * 1‑based array; cache[0] is never used.
 *
 * Build helpers that the surrounding framework supplies are used:
 *   build_primes_upto(limit, &primes, &cnt)  -- get all primes ≤ limit
 *   factor_u64(n, factors, exponents, &cnt)  -- prime factorisation of n
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

/* ---------- static state ---------- */
static seq_t *cache   = NULL;   /* 1‑based cache, cache[0] unused          */
static size_t cache_len  = 0;    /* number of stored terms (excluding 0th)  */
static size_t cache_cap  = 0;    /* capacity of the allocated array         */
static uint64_t last_checked = 0;/* last integer examined                  */

static uint64_t *primes   = NULL;            /* primes[0]==2, primes[1]==3, …          */
static size_t    prime_cnt = 0;              /* number of primes                       */
static uint64_t  prime_limit = 0;            /* largest prime currently in primes      */

/* ---------- helpers ---------- */

/* Ensure primes array contains all primes ≤ limit. */
/* The helper frees the old array and replaces it with a new one. */
static void
ensure_primes_up_to(uint64_t limit)
{
    if (limit <= prime_limit) return;

    uint64_t *new_arr = NULL;
    size_t   new_cnt = 0;
    build_primes_upto(limit, &new_arr, &new_cnt);
    free(primes);
    primes    = new_arr;
    prime_cnt = new_cnt;
    prime_limit = limit;
}

/* Return the 1‑based index of the prime p in the primes array.
 * p is guaranteed to be prime. */
static unsigned int
prime_index_of(uint64_t p)
{
    ensure_primes_up_to(p);
    /* simple binary search */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (unsigned int)(lo + 1);   /* indices are 1‑based */
}

/* Extend cache to contain at least n_needed terms. */
static void
recompute_cache_to(idx_t n_needed)
{
    if (n_needed <= cache_len) return;

    /* Allocate or grow the cache array.  Allocate one extra slot for
     * the 1‑based indexing. */
    if (cache_cap == 0) {
        cache_cap = n_needed * 2 + 1;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) abort();
    } else if (cache_len + n_needed > cache_cap) {
        cache_cap = (cache_cap + n_needed) * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Temporary storage for factorisation results. */
    uint64_t  factors[64];
    uint64_t  exponents[64];
    size_t    factor_cnt = 0;

    while (cache_len < (size_t)n_needed) {
        ++last_checked;

        /* Factorise last_checked.  For n==1 factor_cnt will be 0. */
        factor_u64(last_checked, factors, exponents, &factor_cnt);

        /* Check squarefree: all exponents must be 1. */
        bool squarefree = true;
        for (size_t i = 0; i < factor_cnt; ++i) {
            if (exponents[i] > 1) { squarefree = false; break; }
        }
        if (!squarefree) continue;   /* skip non‑squarefree number */

        /* Sum prime indices of all factors. */
        seq_t sum = 0;
        for (size_t i = 0; i < factor_cnt; ++i) {
            unsigned int idx = prime_index_of(factors[i]);
            sum += (seq_t)idx;
        }

        /* Store the computed term. */
        ++cache_len;
        cache[cache_len] = sum;
    }
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of domain guard */
    recompute_cache_to(n);
    return cache[n];
}
