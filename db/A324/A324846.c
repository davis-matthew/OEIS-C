/*
 * A324846  Positive integers divisible by none of their prime indices.
 * https://oeis.org/A324846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth positive integer not divisible by any index of 
   its prime divisors.  (OEIS A324846) */

#include <stdlib.h>
#include <stdbool.h>

/* Utility functions provided by the build environment. */
#include "../utils/build_spf.h"
#include "../utils/build_primes_upto.h"

static bool _initialized = false;
static const idx_t _LIMIT = 2000000;        /* sufficient for n <= 10 000 */
static idx_t * _spf = NULL;                /* smallest prime factor table */
static idx_t * _prime_index = NULL;        /* prime -> 1‑based index */
static idx_t  _prime_count = 0;
static idx_t * _primes = NULL;             /* list of primes up to _LIMIT */

static idx_t    _next_candidate = 1;      /* next number to test */
static idx_t    _found = 0;               /* how many terms found */
static seq_t *  _cache = NULL;            /* cache of found terms */
static size_t   _cache_cap = 0;           /* capacity of _cache */

/* Ensure auxiliary tables are built once. */
static void _ensure_tables(void)
{
    if (_initialized) return;
    _initialized = true;

    /* Build SPF table up to _LIMIT. */
    build_spf(_LIMIT, &_spf);

    /* Build list of primes up to _LIMIT. */
    build_primes_upto(_LIMIT, &_primes, &_prime_count);

    /* Allocate index table and fill it. */
    _prime_index = (idx_t *)malloc((_LIMIT + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= _LIMIT; ++i) _prime_index[i] = 0;
    for (idx_t k = 0; k < _prime_count; ++k) {
        _prime_index[_primes[k]] = k + 1;        /* prime(1)=2 */
    }
}

/* Test whether num (of type seq_t) is accepted by the sequence rule. */
static bool _good_candidate(seq_t num)
{
    /* Even numbers are automatically excluded because they are divisible by 2
       (prime(1)) and 1 divides every integer. */
    if ((num & 1) == 0) return false;

    idx_t tmp = (idx_t)num;                /* tmp will be divided down. */
    idx_t last_p = 0;
    while (tmp > 1) {
        idx_t p = _spf[tmp];
        if (p != last_p) {                /* unique prime factor */
            idx_t k = _prime_index[p];
            if ((num % (seq_t)k) == 0)    /* fails the rule */
                return false;
            last_p = p;
        }
        tmp /= p;
    }
    return true;                          /* passes all checks */
}

/* Main function requested by the infrastructure. */
seq_t a(idx_t n)
{
    /* n is 1‑based; ensure that the cache holds at least n terms. */
    _ensure_tables();

    while (_found < n) {
        seq_t cand = (seq_t)_next_candidate;
        if (_good_candidate(cand)) {
            /* store into cache */
            if (_found >= _cache_cap) {
                size_t new_cap = _cache_cap ? _cache_cap * 2 : 128;
                seq_t *new_arr = (seq_t *)realloc(_cache,
                                                  new_cap * sizeof(seq_t));
                _cache = new_arr;
                _cache_cap = new_cap;
            }
            _cache[_found++] = cand;
        }
        _next_candidate += 2;              /* skip even numbers */
    }
    return _cache[n - 1];
}
