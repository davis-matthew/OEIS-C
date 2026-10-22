/*
 * A392368  Numbers whose least prime factor is a Fermat prime.
 * https://oeis.org/A392368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose least prime factor is a Fermat prime.
   a(n) = the n‑th natural number k (k≥3) whose least prime divisor is
   one of 3,5,17,257,65537. */

/*  The table of Fermat primes that are used in the definition.
    They are checked for membership during the search. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Fermat primes up to 65537.  Add more if desired. */
static const uint64_t _fermat_primes[] = {3ULL,5ULL,17ULL,257ULL,65537ULL};
static const size_t _FERMAT_CNT = sizeof _fermat_primes / sizeof _fermat_primes[0];

/* Return true if p is one of the Fermat primes above. */
static bool _is_fermat_prime(uint64_t p) {
    for (size_t i = 0; i < _FERMAT_CNT; ++i) {
        if (p == _fermat_primes[i]) return true;
    }
    return false;
}

/* Test whether the least prime factor of n is a Fermat prime.
   For n >= 3.  The function performs trial division up to sqrt(n). */
static bool _has_fermat_lpf(uint64_t n) {
    if (n <= 2) return false;
    if ((n & 1U) == 0) return false;         /* even numbers → lpf = 2 */

    uint64_t d;
    /* Check odd divisors starting at 3.  We stop when d > n/d to avoid overflow. */
    for (d = 3; d <= n / d; d += 2) {
        if (n % d == 0) {
            /* Found the smallest prime divisor. */
            return _is_fermat_prime(d);
        }
    }
    /* No divisor found ⇒ n is prime itself. */
    return _is_fermat_prime(n);
}

/* Cache of computed terms.  The cache is indexed from 0 to count-1. */
static seq_t * _cache = NULL;
static idx_t  _cache_count = 0;
static size_t _cache_cap   = 0;

/* Ensure that at least target_n terms are cached. */
static void _ensure_cache(idx_t target_n) {
    if (_cache == NULL) {
        _cache_cap   = 128;
        _cache       = (seq_t *)malloc(_cache_cap * sizeof(seq_t));
        _cache_count = 0;
    }
    while (_cache_count < target_n) {
        uint64_t cand;
        if (_cache_count == 0) {
            cand = 3;              /* the smallest k with the property */
        } else {
            cand = (uint64_t)_cache[_cache_count - 1] + 1ULL;
        }

        /* Find the next k whose lpf is a Fermat prime. */
        while (!_has_fermat_lpf(cand)) {
            ++cand;
        }

        /* Store the term. */
        if (_cache_count >= _cache_cap) {
            _cache_cap *= 2;
            _cache = (seq_t *)realloc(_cache, _cache_cap * sizeof(seq_t));
        }
        _cache[_cache_count++] = (seq_t)cand;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;              /* undefined for n=0, return 0 for safety */
    _ensure_cache(n);
    return _cache[n - 1];
}
