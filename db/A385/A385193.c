/*
 * A385193  Odd multiplicative orders of 5 modulo primes.
 * https://oeis.org/A385193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the multiplicative order of 5 modulo the n‑th prime *
 *            for which that order is odd. The function searches primes in
 *            order, * keeps the odd orders in a cache, and returns the n‑th
 *            cached value
 * Range:     u8 n <= 20, u16 n <= 1035, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime, mod_pow, next_prime_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"     /* mod_pow(base, exp, mod) */
#include "../utils/factor_u64.h"  /* factor_u64(n, primes, exps, &cnt) */
#include "../utils/is_prime.h"    /* is_prime(idx_t x) */
#include "../utils/next_prime_candidate.h"

static seq_t *cache          = NULL;   /* a(1), a(2), ... */
static idx_t  cache_len      = 0;
static idx_t  cache_cap      = 0;

static uint64_t *prime_list  = NULL;   /* primes encountered so far */
static size_t   prime_count  = 0;
static size_t   prime_cap    = 0;

/* Ensure cache has capacity for at least `need` elements. */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* Ensure prime list has capacity for at least `need` primes. */
static void grow_primes(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *new_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!new_list) abort();
    prime_list = new_list;
    prime_cap = new_cap;
}

/* Compute the multiplicative order of 5 modulo the prime `p`.
 * The result fits into a `uint64_t` (and is later cast to `seq_t`). */
static uint64_t order_of_5(uint64_t p)
{
    uint64_t ord = p - 1;          /* start with p-1 */
    uint64_t fac[64], exps[64];
    size_t cnt = 0;
    factor_u64(ord, fac, exps, &cnt);   /* factor p-1 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = fac[i];
        /* divide out powers of q as long as the condition holds */
        while (ord % q == 0) {
            uint64_t cand = ord / q;
            seq_t r = mod_pow((seq_t)5, cand, (seq_t)p);
            if (r == (seq_t)1) {
                ord = cand;
            } else {
                break;
            }
        }
    }
    return ord;
}

/* Main sequence function.  Uses static caching so that calls are
 * fast regardless of order.  Builds primes lazily as needed. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; guard clause */

    /* Extend cache up to n if necessary */
    if (cache_len < n) {
        grow_cache(n);

        /* Start searching primes from the last one we saw (or 1). */
        static uint64_t last_seen = 1;

        for (idx_t idx = cache_len; idx < n; ++idx) {
            for (;;) {
                /* find next prime */
                if (!next_prime_candidate(&last_seen)) abort();

                if (!is_prime(last_seen)) continue;

                /* Record prime */
                grow_primes(prime_count + 1);
                prime_list[prime_count++] = last_seen;

                /* Compute order of 5 mod this prime */
                uint64_t ord = order_of_5(last_seen);
                if ((ord & 1ULL) == 0ULL) {
                    /* even order – skip */
                    continue;
                }

                /* odd order – store in cache */
                cache[cache_len++] = (seq_t)ord;
                break;
            }
        }
    }

    return cache[n - 1];
}
