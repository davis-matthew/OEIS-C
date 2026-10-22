/*
 * A397754  Numbers that don't contain digit 0 but have a divisor that does.
 * https://oeis.org/A397754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _contains_digit0_uint64, _dfs_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A397754: Numbers that don't contain digit 0 but have a divisor that does. 
   Computed by incremental search with memoization. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_contains_digit0_uint64.h"
#include "../utils/_dfs_divisors.h"

/* ----- miscellaneous utilities -------------------------------------------------- */

static uint32_t _primes[100000];
static size_t _prime_count = 0;

/* initial simple sieve to generate primes up to 1,000,000 (enough for factorisation). */
static void _init_primes(void) {
    static bool done = false;
    if (done) return;
    static unsigned char _sieve[1000001] = {0};
    _prime_count = 0;
    for (uint32_t i = 2; i <= 1000000; ++i) {
        if (!_sieve[i]) {
            _primes[_prime_count++] = i;
            if ((uint64_t)i * i <= 1000000) {
                for (uint32_t j = i * i; j <= 1000000; j += i)
                    _sieve[j] = 1;
            }
        }
    }
    done = true;
}

/* factorise n into prime factors and exponents (max 20 factors). */
static void _factor_uint64(uint64_t n, uint64_t *factors, int *exps, size_t *cnt) {
    _init_primes();
    size_t c = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < _prime_count && (uint64_t)_primes[i] * (uint64_t)_primes[i] <= tmp; ++i) {
        if (tmp % _primes[i] == 0) {
            int e = 0;
            while (tmp % _primes[i] == 0) {
                tmp /= _primes[i];
                ++e;
            }
            factors[c] = _primes[i];
            exps[c] = e;
            ++c;
        }
    }
    if (tmp > 1) {
        factors[c] = tmp;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* true iff n (which does not contain digit 0) has at least one divisor that contains digit 0. */
static bool _divisor_has_digit0(uint64_t n) {
    uint64_t factors[20];
    int exps[20];
    size_t cnt;
    _factor_uint64(n, factors, exps, &cnt);
    bool found = false;
    _dfs_divisors(1, 0, cnt, factors, exps, &found);
    return found;
}

/* ----- cache for computed sequence --------------------------------------------- */

static seq_t * _cache = NULL;
static size_t _cache_cap = 0;
static size_t _cache_size = 0;
static uint64_t _next_candidate = 1;

/* ensure cache has space for at least 'need' elements */
static void _grow_cache(size_t need) {
    if (_cache_cap >= need) return;
    size_t new_cap = _cache_cap ? _cache_cap * 2 : 256;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
    if (!tmp) { free(_cache); exit(1); }
    _cache = tmp;
    _cache_cap = new_cap;
}

/* ----- final sequence function ------------------------------------------------ */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset handling */

    _grow_cache((size_t)n);

    while (_cache_size < (size_t)n) {
        /* advance until a valid candidate is found */
        while (true) {
            if (!_contains_digit0_uint64(_next_candidate)) {
                if (_divisor_has_digit0(_next_candidate)) {
                    _cache[_cache_size++] = (seq_t)_next_candidate;
                    ++_next_candidate;          /* skip the value that was just accepted */
                    break;
                }
            }
            ++_next_candidate;
        }
    }
    return _cache[(size_t)n - 1];
}
