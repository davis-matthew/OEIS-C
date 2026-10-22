/*
 * A341658  Primes p such that p^2 - 1 has 32 divisors.
 * https://oeis.org/A341658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p such that p^2 − 1 has exactly 32 divisors
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* helper from utils */
#include "../utils/seq_realloc.h"

/* Static storage for primes and cache */
static uint32_t *prime_arr = NULL;      /* list of primes up to current sieve_limit */
static size_t prime_cnt = 0;            /* number of primes in prime_arr      */
static size_t prime_idx = 0;            /* next prime to examine              */
static size_t sieve_limit = 0;          /* upper bound of current sieve       */

static seq_t *cache = NULL;             /* array of found terms               */
static size_t cache_len = 0;            /* number of terms stored             */
static size_t cache_cap = 0;            /* capacity of cache array            */

/*---------------------------------------------------------------*/
/* Sieve of Eratosthenes: create prime list up to 'limit'.      */
/* Rebuilds the sieve completely if a larger limit is requested.*/
/*---------------------------------------------------------------*/
static void build_sieve(size_t limit)
{
    if (limit <= sieve_limit) return;   /* nothing to do */

    /* allocate temporary array for composites */
    uint8_t *comp = (uint8_t *)calloc(limit + 1, 1);
    for (size_t i = 2; i * i <= limit; ++i)
        if (!comp[i])
            for (size_t j = i * i; j <= limit; j += i)
                comp[j] = 1;

    /* count primes and allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!comp[i]) ++cnt;

    uint32_t *pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t k = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!comp[i]) pr[k++] = (uint32_t)i;

    /* replace old data */
    free(prime_arr);
    free(comp);
    prime_arr = pr;
    prime_cnt = cnt;
    sieve_limit = limit;
}

/*---------------------------------------------------------------*/
/* Factor m and return divisor count and exponent of prime 2.    */
/* Uses prime_arr which must contain all primes up to sqrt(m).   */
/*---------------------------------------------------------------*/
static void factor_divcnt_exp2(uint64_t m, uint64_t *div_cnt, uint32_t *e2)
{
    uint64_t d = 1;
    uint32_t e = 0;
    uint64_t t = m;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_arr[i] * prime_arr[i] <= t; ++i) {
        uint32_t p = prime_arr[i];
        if (t % p == 0) {
            uint32_t exp = 0;
            while (t % p == 0) {
                t /= p;
                ++exp;
            }
            d *= (exp + 1);
            if (p == 2) e = exp;
        }
    }
    if (t > 1) {            /* remaining prime factor */
        d *= 2;
        if (t == 2) e += 1;
    }
    *div_cnt = d;
    *e2     = e;
}

/*---------------------------------------------------------------*/
/* Check if prime p satisfies the sequence condition.           */
/* Returns true iff (p^2 - 1) has exactly 32 divisors.          */
/*---------------------------------------------------------------*/
static bool has_property(uint64_t p)
{
    uint64_t d1, d2;
    uint32_t e1, e2;
    factor_divcnt_exp2(p - 1, &d1, &e1);
    factor_divcnt_exp2(p + 1, &d2, &e2);

    __int128 comb = (__int128)d1 * d2;
    if (e1 > 0 && e2 > 0) {
        comb = comb / ((e1 + 1) * (e2 + 1)) * (e1 + e2 + 1);
    }
    return (comb == 32);
}

/*---------------------------------------------------------------*/
/* Ensure that cache[n] is at least filled up to index 'n'.     */
/* Provides an O(n) construction sufficient for the limit ~10⁴.*/
/*---------------------------------------------------------------*/
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        /* Grow sieve if needed to examine next prime */
        if (prime_idx >= prime_cnt) {
            size_t new_limit = sieve_limit ? sieve_limit * 2 : 1000;
            /* a rough upper bound based on requested n */
            size_t est_limit = (size_t)(n * 250 + 1000);
            if (est_limit > new_limit) new_limit = est_limit;
            build_sieve(new_limit);
        }

        /* Process remaining primes in the current sieve */
        for (; prime_idx < prime_cnt && cache_len < (size_t)n; ++prime_idx) {
            uint64_t p = prime_arr[prime_idx];
            if (p <= 3) continue;        /* primes 2 and 3 are excluded */
            if (has_property(p)) {
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)p;
            }
        }
    }
}

/*---------------------------------------------------------------*/
/* Public sequence function.                                     */
/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for index 0, but guard */
    ensure_cache(n);
    return cache[n - 1];
}
