/*
 * A367947  Semiprimes s such that 2*s+1 is prime.
 * https://oeis.org/A367947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A367947:  semiprimes s such that 2*s+1 is prime. */
/*  a(n) = s, with a(1)=6, a(2)=9, ...  (index 1‑based).  */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

/* cache for the computed sequence */
static seq_t  *cache      = NULL;   /* array of sequence values          */
static size_t  cache_len  = 0;      /* number of cached values           */
static size_t  cache_cap  = 0;      /* allocated capacity (>= cache_len) */
static seq_t   next_s     = 6;      /* next integer to test             */

/* enlarge cache if needed */
static void grow_cache_if_needed(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_mem;
    cache_cap = new_cap;
}

/* Test whether a 64‑bit integer n is a semiprime */
static bool is_semiprime(uint64_t n)
{
    /* factor the number */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(n, primes, exps, &cnt);

    /* sum of prime exponents must be exactly 2 */
    size_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) sum += exps[i];
    if (sum != 2) return false;

    /* check that 2*n+1 is prime */
    __uint128_t t128 = (__uint128_t)n * 2 + 1;
    if (t128 >> 64) return false;          /* cannot test, treat as non‑prime */
    idx_t t = (idx_t)(t128 & 0xFFFFFFFFFFFFFFFFULL);
    return is_prime(t);
}

/* extend cache until we have at least 'target' elements */
static void ensure_cache_size(size_t target)
{
    if (cache_len >= target) return;

    grow_cache_if_needed(target);

    while (cache_len < target) {
        uint64_t cand = (uint64_t)next_s;          /* candidate as uint64_t */
        if (is_semiprime(cand)) {
            cache[cache_len++] = next_s;
        }
        ++next_s;
    }
}

/* required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* symbolic undefined value   */
    ensure_cache_size((size_t)n);
    return cache[(size_t)n - 1];
}
