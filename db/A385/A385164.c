/*
 * A385164  Let p = A002145(n) be the n-th prime == 3 (mod 4); 8*a(n) is the
 *          multiplicative order of 1+-i modulo p in Gaussian integers.
 * https://oeis.org/A385164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ord(-4,p)/2 where p is the n‑th prime congruent 3 (mod 4)
 * Range:     u8 n <= 30, u16 n <= 4371, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, mod_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"                     /* mod_pow(seq_t, uint64_t, seq_t) */
#include "../utils/_smallest_prime_factor.h"     /* _smallest_prime_factor(uint64_t) */
#include "../utils/seq_realloc.h"                /* seq_realloc(seq_t*, size_t, size_t) */

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static bool  *computed = NULL;
static size_t computed_cap = 0;

static uint64_t *primes_all = NULL;
static size_t primes_all_cnt = 0;
static size_t primes_all_cap = 0;

static uint64_t *primes3   = NULL;
static size_t   primes3_cnt = 0;
static size_t   primes3_cap = 0;

static uint64_t next_candidate = 3;            /* next candidate odd number */

/* Ensure cache can hold index n (>=n+1).  Zero‑initialize new slots. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = n + 1;
    if (new_cap < 64) new_cap = 64;
    cache = seq_realloc(cache, cache_cap, new_cap);
    for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
    cache_cap = new_cap;
}

/* Ensure computed flag array can hold index n (>=n+1).  Zero‑initialize new slots. */
static void ensure_computed(idx_t n)
{
    if ((size_t)n < computed_cap) return;
    size_t new_cap = n + 1;
    if (new_cap < 64) new_cap = 64;
    bool *tmp = (bool*)realloc(computed, new_cap * sizeof(bool));
    if (!tmp) exit(1);
    for (size_t i = computed_cap; i < new_cap; ++i) tmp[i] = false;
    computed = tmp;
    computed_cap = new_cap;
}

/* Ensure primes3 contains at least n elements.  Generates primes incrementally
   using trial division by previously found primes. */
static void ensure_primes3(idx_t n)
{
    if (primes3_cnt >= n) return;

    /* initialise prime lists if first time */
    if (!primes_all) {
        primes_all_cap = 16;
        primes_all = (uint64_t*)malloc(primes_all_cap * sizeof(uint64_t));
        if (!primes_all) exit(1);
        primes_all_cnt = 0;
        primes_all[primes_all_cnt++] = 2;    /* 2 is the first prime */
    }
    if (!primes3) {
        primes3_cap = 16;
        primes3 = (uint64_t*)malloc(primes3_cap * sizeof(uint64_t));
        if (!primes3) exit(1);
        primes3_cnt = 0;
    }

    uint64_t cand = next_candidate;      /* next candidate to test */
    while (primes3_cnt < n) {
        /* find next prime >= cand */
        bool is_prime = false;
        while (!is_prime) {
            is_prime = true;
            for (size_t i = 0; i < primes_all_cnt; ++i) {
                uint64_t p = primes_all[i];
                if (p * p > cand) break;
                if (cand % p == 0) { is_prime = false; break; }
            }
            if (!is_prime) cand += 2;     /* next odd number */
        }
        /* cand is prime */
        if (primes_all_cnt >= primes_all_cap) {
            primes_all_cap <<= 1;
            primes_all = (uint64_t*)realloc(primes_all, primes_all_cap * sizeof(uint64_t));
            if (!primes_all) exit(1);
        }
        primes_all[primes_all_cnt++] = cand;

        if ((cand & 3) == 3) {            /* cand % 4 == 3 */
            if (primes3_cnt >= primes3_cap) {
                primes3_cap <<= 1;
                primes3 = (uint64_t*)realloc(primes3, primes3_cap * sizeof(uint64_t));
                if (!primes3) exit(1);
            }
            primes3[primes3_cnt++] = cand;
        }
        cand += 2;                         /* prepare next candidate */
        next_candidate = cand;
    }
}

/* Computes the multiplicative order of g modulo prime p.  p is an
   odd prime.  The order divides p-1. */
static seq_t order_of_g(uint64_t g, uint64_t p)
{
    uint64_t ord = p - 1;                 /* starting order */
    uint64_t fac[64];
    size_t fac_cnt = 0;
    uint64_t tmp = ord;
    /* factor ord into unique prime factors using _smallest_prime_factor */
    while (tmp > 1) {
        uint64_t spf = _smallest_prime_factor(tmp);
        if (fac_cnt == 0 || fac[fac_cnt - 1] != spf) fac[fac_cnt++] = spf;
        while (tmp % spf == 0) tmp /= spf;
    }
    /* reduce the order using the prime factors */
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t q = fac[i];
        while (ord % q == 0) {
            uint64_t exp = ord / q;
            if (mod_pow((seq_t)g, exp, (seq_t)p) == (seq_t)1) {
                ord = exp;
            } else break;
        }
    }
    return (seq_t)(ord / 2);              /* a(n) = ord(-4,p)/2 */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset starts at 1 */
    ensure_cache(n);
    ensure_computed(n);
    if (computed[n]) return cache[n];      /* already computed */

    ensure_primes3(n);
    uint64_t p = primes3[n - 1];            /* nth prime ≡3 (mod 4) */

    uint64_t g = p - 4;                     /* -4 mod p, p>4 so g>0 */
    seq_t result = order_of_g(g, p);        /* a(n) = ord(-4,p)/2 */

    cache[n] = result;
    computed[n] = true;
    return result;
}
