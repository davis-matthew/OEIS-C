/*
 * A374114  Numbers k such that A113177(k) and A276085(k) are both even, where
 *          A113177 and A276085 are fully additive with a(p) = Fibonacci(p)
 *          and a(p) = p#/p, respectively.
 * https://oeis.org/A374114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 9913, u32 n <= 9913, u64 n <= 9913,
 *            u128 n <= 9913, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374114:
 * Numbers k such that A113177(k) and A276085(k) are both even,
 * i.e. the 2‑adic valuation of k is even and the parity of the
 * number of prime factors (with multiplicity) equals the parity
 * of the 3‑adic valuation of k.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* Prime sieve – sufficient for k <= ~10^6 (more than enough for n <= 10000) */
static bool _sieve_initialised = false;
static uint64_t _primes[200];
static size_t _prime_cnt = 0;

static void _init_primes(void)
{
    if (_sieve_initialised) return;
    const size_t LIMIT = 1000;          /* 1000 > sqrt(10^6) */
    bool *is_comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;
        }
    }
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) _primes[idx++] = i;
    _prime_cnt = idx;
    free(is_comp);
    _sieve_initialised = true;
}

/* --------------------------------------------------------------------- */
/* Check if k satisfies the condition */
static bool _valid_k(uint64_t k)
{
    if (k == 1) return true;          /* 1 is always in the sequence */

    bool v2_even = true;              /* parity of exponent of 2 */
    int  omega_parity = 0;            /* parity of Ω(k) (total exponents) */
    int  v3_parity   = 0;             /* parity of exponent of 3 */

    uint64_t n = k;
    for (size_t ip = 0; ip < _prime_cnt && _primes[ip] * _primes[ip] <= n; ++ip) {
        uint64_t p = _primes[ip];
        if (n % p == 0) {
            size_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            int epar = exp & 1;       /* exp % 2 */
            omega_parity ^= epar;
            if (p == 3) v3_parity = epar;
            if (p == 2) v2_even = (epar == 0);
        }
    }
    if (n > 1) {                      /* remaining prime factor */
        int epar = 1;
        omega_parity ^= epar;
        if (n == 3) v3_parity = epar;
        if (n == 2) v2_even = (epar == 0);
    }

    return v2_even && ((omega_parity ^ v3_parity) == 0);
}

/* --------------------------------------------------------------------- */
/* Sequence cache */
static seq_t * _cache = NULL;
static idx_t  _cache_sz = 0;
static idx_t  _cache_cap = 0;
static uint64_t _next_candidate = 1;
static bool _init_done = false;

static void _ensure_cache_capacity(idx_t need)
{
    if (_cache_cap >= need) return;
    idx_t new_cap = _cache_cap ? _cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* critical error – abort */
        abort();
    }
    _cache = new_cache;
    _cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Main function implementing the sequence */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (!_init_done) {
        _init_primes();
        _ensure_cache_capacity(16);
        _init_done = true;
    }

    /* grow the cache until we have the nth element */
    while (_cache_sz < n) {
        while (!_valid_k(_next_candidate))
            ++_next_candidate;

        _ensure_cache_capacity(_cache_sz + 1);
        _cache[_cache_sz++] = (seq_t)_next_candidate;
        ++_next_candidate;
    }

    return _cache[n - 1];
}
