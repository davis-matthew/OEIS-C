/*
 * A318718  Heinz numbers of strict integer partitions with a common divisor >
 *          1.
 * https://oeis.org/A318718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th Heinz number of a strict integer partition
 *            whose parts have a common divisor > 1. Equivalently, a(n) is the
 *            n‑th square‑free integer whose prime‑factor indices all share a
 *            divisor > 1
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   _gcd32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_gcd32.h"

/* global initialization flag and tables */
static bool          _initialized = false;
static uint32_t     * _spf         = NULL;          /* smallest prime factor */
static uint32_t     * _prime_idx   = NULL;          /* 1‑based index of a prime */
static uint32_t        _limit      = 3000000U;      /* sieve limit */

static seq_t        * _cache       = NULL;          /* sequence cache */
static size_t        _cache_len   = 0;
static size_t        _cache_cap   = 0;
static uint32_t      _next_check  = 3;              /* next number to scan */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Build SPF and prime index arrays up to _limit */
static void _build_sieve(void)
{
    _spf = (uint32_t *)malloc((_limit + 1) * sizeof(uint32_t));
    _prime_idx = (uint32_t *)calloc((_limit + 1), sizeof(uint32_t));
    if (!_spf || !_prime_idx) abort();

    for (uint32_t i = 2; i <= _limit; ++i) _spf[i] = i;

    /* Classic Eratosthenes sieve for SPF */
    for (uint32_t i = 2; (uint64_t)i * (uint64_t)i <= _limit; ++i) {
        if (_spf[i] == i) { /* i is prime */
            for (uint32_t j = i * i; j <= _limit; j += i) {
                if (_spf[j] == j) _spf[j] = i;
            }
        }
    }

    /* Fill 1‑based prime indices */
    uint32_t idx = 1;
    for (uint32_t i = 2; i <= _limit; ++i) {
        if (_spf[i] == i) {          /* i is prime */
            _prime_idx[i] = idx++;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Test if an integer < = _limit satisfies the sequence condition */
/* – square‑free, prime indices all >1, and gcd of indices >1 */
static bool _qualifies(uint32_t n)
{
    if (n < 3U) return false;          /* sequence starts at 3 */

    uint32_t g = 0U;                   /* current gcd of indices */
    while (n > 1U) {
        uint32_t p = _spf[n];
        uint32_t exp = 0U;
        while (n % p == 0U) {
            n /= p;
            ++exp;
            if (exp > 1U) return false;      /* not square‑free */
        }
        uint32_t pi = _prime_idx[p];
        if (pi <= 1U) return false;          /* prime 2 or error */
        if (g == 0U) g = pi;
        else        g = _gcd32(g, pi);
        if (g == 1U) return false;           /* gcd dropped to 1 */
    }
    return g > 1U;
}

/* ------------------------------------------------------------------ */
/* Extend the cache until it contains at least 'n' terms */
static void _extend_cache(idx_t n)
{
    /* Ensure the buffer can hold 'n' elements */
    if (_cache_cap < (size_t)n) {
        size_t new_cap = _cache_cap == 0 ? 256 : _cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        _cache = tmp;
        _cache_cap = new_cap;
    }

    /* Scan integers and collect qualifying ones */
    while (_cache_len < (size_t)n) {
        if (_next_check > _limit) {
            abort(); /* limit exceeded – should not happen for required terms */
        }
        if (_qualifies(_next_check)) {
            _cache[_cache_len++] = (seq_t)_next_check;
        }
        ++_next_check;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined position 0 */

    if (!_initialized) {
        _build_sieve();
        _initialized = true;
    }

    _extend_cache(n);
    return _cache[(size_t)(n - 1)];
}
