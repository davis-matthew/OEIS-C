/*
 * A323628  Primes p such that 3 is a primitive root modulo p while 2187 is
 *          not.
 * https://oeis.org/A323628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that * 1) 3 is a primitive root
 *            modulo p, * 2) p ≡ 1 (mod 7)
 * Range:     u8 n <= 6, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility routines that are available in
 * the OEIS build environment */
#include "../utils/is_prime.h"     /* bool is_prime(idx_t n); */
#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

/* ------------------------------------------------------------------ */
/* integer square root for 64‑bit values (non‑floating point)        */
static inline idx_t isqrt_u64(uint64_t n)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 63;
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ------------------------------------------------------------------ */
/* prime table: dynamic sieve that grows as needed                     */
static uint64_t *prime_list   = NULL;
static size_t    prime_cnt    = 0;
static size_t    prime_cap    = 0;
static uint8_t  *sieve_flag   = NULL;
static uint64_t  sieve_size   = 0;

/* enlarge the sieve so that all primes <= limit are known            */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= sieve_size) return;

    uint64_t new_size = limit + 1;
    uint8_t *new_flag = (uint8_t*)realloc(sieve_flag, new_size);
    if (!new_flag) abort();                /* allocation failure */

    /* initialise new entries to 0 (prime candidate)                */
    for (uint64_t i = sieve_size; i < new_size; ++i)
        new_flag[i] = 0;

    /* mark composites using the primes already known               */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        uint64_t start = (sieve_size + p - 1) / p;
        start = start * p;
        if (start < p * p) start = p * p;
        for (uint64_t j = start; j < new_size; j += p)
            new_flag[j] = 1;
    }

    /* extend the sieve with new region                           */
    for (uint64_t num = (sieve_size > 2 ? sieve_size : 2);
         num < new_size; ++num)
    {
        if (!new_flag[num]) {
            /* num is prime                                         */
            if (prime_cnt == prime_cap) {
                size_t new_cap = (prime_cap == 0) ? 8 : prime_cap * 2;
                uint64_t *tmp = (uint64_t*)realloc(prime_list,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp) abort();            /* allocation failure */
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_cnt++] = num;

            if (num * num < new_size) {
                for (uint64_t j = num * num; j < new_size; j += num)
                    new_flag[j] = 1;
            }
        }
    }

    sieve_size  = new_size;
    sieve_flag  = new_flag;
}

/* ------------------------------------------------------------------ */
/* test whether 3 is a primitive root modulo a prime p                */
static bool is_primitive_root(uint64_t p)
{
    uint64_t m     = p - 1;                /* φ(p) */
    uint64_t temp  = m;
    uint64_t sqrtm = isqrt_u64(temp);
    ensure_primes_up_to(sqrtm);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_list[i];
        if (q > temp) break;
        if (temp % q == 0) {
            seq_t res = mod_pow((seq_t)3, (uint64_t)(m / q), (seq_t)p);
            if (res == (seq_t)1) return false;
            while (temp % q == 0) temp /= q;
        }
    }
    if (temp > 1) {
        seq_t res = mod_pow((seq_t)3, (uint64_t)(m / temp), (seq_t)p);
        if (res == (seq_t)1) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* cache of sequence values                                            */
static seq_t *cache       = NULL;
static size_t cache_cnt   = 0;
static size_t cache_cap   = 0;
static uint64_t next_candidate = 2;      /* next number to test          */

/* fill the cache until it contains n values                         */
static void extend_cache(idx_t n)
{
    while (cache_cnt < n) {
        /* find next number of the form 7k+1                            */
        do {
            ++next_candidate;
        } while (next_candidate % 7 != 1);

        if (!is_prime(next_candidate)) continue;

        if (!is_primitive_root(next_candidate)) continue;

        /* grow cache if necessary                                     */
        if (cache_cnt == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();                /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_cnt++] = (seq_t)next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* public sequence function                                           */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour below offset is undefined */
    extend_cache(n);
    return cache[n - 1];
}
