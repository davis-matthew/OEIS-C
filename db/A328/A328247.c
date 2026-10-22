/*
 * A328247  Numbers whose third arithmetic derivative (A099306) is a
 *          squarefree number (A005117), but the second derivative (A068346)
 *          is not.
 * https://oeis.org/A328247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 571, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A328247: Numbers whose third arithmetic derivative is squarefree
 * but whose second derivative is not.  The routine computes each term
 * by testing successive integers and caching the results so that
 * subsequent calls (in any order) have O(1) lookup time.
 */

/* standard headers */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

/* utilities for the arithmetic derivative and squarefree test */
#include "../utils/is_squarefree.h"

/* ------------------------------------------------------------------ */
/* Prime sieve and storage (used for integer factorisation)          */
/* ------------------------------------------------------------------ */
static uint64_t *primes = NULL;
static size_t   primes_cnt = 0;
static uint64_t primes_limit = 0;

/* Simple Eratosthenes sieve up to 'limit' (inclusive)                */
static void build_primes(uint64_t limit)
{
    if (limit <= primes_limit) return;
    /* allocate bit array */
    uint8_t *is_comp = (uint8_t *)malloc(limit + 1);
    if (!is_comp) abort();
    is_comp[0] = is_comp[1] = 1;
    for (uint64_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
    /* count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++cnt;
    uint64_t *new_p = (uint64_t *)realloc(primes, cnt * sizeof(uint64_t));
    if (!new_p) abort();
    primes = new_p;
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) primes[idx++] = i;
    primes_cnt = cnt;
    primes_limit = limit;
    free(is_comp);
}

/* initialise a reasonably large list of primes */
static void init_primes(void)
{
    static bool done = false;
    if (!done) {
        build_primes(1000000ULL);          /* up to 1 e6 */
        done = true;
    }
}

/* ------------------------------------------------------------------ */
/* Arithmetic derivative for unsigned 64‑bit integers                  */
/* ------------------------------------------------------------------ */
static seq_t arith_derivative_u64(uint64_t n)
{
    if (n <= 1)
        return 0;
    uint64_t temp = n;
    seq_t  sum = 0;
    init_primes();
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* add n/p * e, using 128‑bit to avoid overflow */
            sum += (seq_t)((unsigned __int128)(n / p) * e);
        }
    }
    if (temp > 1) {                 /* remaining prime factor of exponent 1 */
        sum += (seq_t)(n / temp);
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/* Sequence cache (memoised)                                         */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static idx_t  cache_cap = 0;
static uint64_t next_candidate = 2;

/* append a value, enlarging the cache if necessary */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Public function required by the build                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for <1, but keep sane */
        return 0;

    /* generate terms until we have at least n entries */
    while (cache_len < n) {
        uint64_t cand = next_candidate++;
        seq_t d1 = arith_derivative_u64(cand);
        uint64_t d1u = (uint64_t)d1;                  /* used for next step */
        seq_t d2 = arith_derivative_u64(d1u);
        if (d2 <= 1) continue;                       /* skip trivial cases */
        uint64_t d2u = (uint64_t)d2;
        seq_t d3 = arith_derivative_u64(d2u);

        if (is_squarefree(d3) && !is_squarefree(d2))
            cache_push((seq_t)cand);
    }

    return cache[n - 1];
}
