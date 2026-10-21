/*
 * A270428  Exponentially odious numbers: 1 together with positive integers n
 *          such that all exponents in prime factorization of n are odious
 *          numbers (A000069).
 * https://oeis.org/A270428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_odious_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exponentially odious numbers (A270428)
   – integers n whose prime‑factor exponents are all odious.
   The function builds a sieve incrementally until it has found
   n terms and then returns the nth term. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_odious_exponent.h"

/*---  global data  -----------------------------------------------------*/
static idx_t *spf = NULL;          /* smallest prime factor array       */
static idx_t  spf_size = 0;        /* current size of spf array          */
static uint32_t *primes = NULL;    /* list of primes for sieving        */
static size_t   primes_cap = 0;
static size_t   primes_len = 0;

static seq_t  *cache = NULL;       /* sequence values                   */
static idx_t  cache_cap = 0;       /* capacity of cache                 */
static idx_t  cache_len = 0;       /* current length of cache           */
static idx_t  next_candidate = 0;  /* next integer to test              */
static bool   initialized = false; /* first‑call flag                    */

/*---  utility: extend spf up to at least `limit`  -----------------------*/
static void
ensure_spf(idx_t limit)
{
    if (spf_size >= limit) return;          /* already large enough */

    free(spf);
    free(primes);

    spf_size = limit;
    spf = (idx_t*)calloc((size_t)(spf_size + 1), sizeof(idx_t));
    primes_cap = 32;
    primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    primes_len = 0;

    for (idx_t i = 2; i <= spf_size; ++i) {
        if (spf[i] == 0) {                      /* i is prime */
            spf[i] = i;
            if (primes_len == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t*)realloc(primes,
                        primes_cap * sizeof(uint32_t));
            }
            primes[primes_len++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_len; ++j) {
            idx_t p = primes[j];
            idx_t ip = i * p;
            if (ip > spf_size) break;
            spf[ip] = p;
            if (p == spf[i]) break;
        }
    }
}

/*---  utility: test if every exponent in factorisation of `x` is odious */
static bool
is_exponentially_odious(idx_t x)
{
    while (x > 1) {
        idx_t p = spf[x];
        idx_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        if (!is_odious_exponent(cnt))
            return false;
    }
    return true;
}

/*---  utility: append a value to the cache  ---------------------------*/
static void
append_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/*---  main sequence function  ----------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not used, but safe */

    if (!initialized) {              /* first call: initialise storage */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 1;
        cache[0] = (seq_t)1;         /* a(1) = 1 */
        next_candidate = 2;
        ensure_spf(next_candidate);
        initialized = true;
    }

    while (cache_len < n) {
        /* test successive integers until a new term is found */
        while (true) {
            if (next_candidate > spf_size) ensure_spf(next_candidate);
            if (is_exponentially_odious(next_candidate)) {
                append_term((seq_t)next_candidate);
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }

    return cache[(size_t)(n - 1)];
}
