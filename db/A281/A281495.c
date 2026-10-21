/*
 * A281495  Least k > 1 such that k^n is a refactorable number.
 * https://oeis.org/A281495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 1 such that k^n is a refactorable number (i.e.,
 *            the number of divisors of k^n divides k^n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PRIMELIMIT 100000

/* Prime sieve (once per program) */
static bool _primes_ready = false;
static uint32_t _primes[PRIMELIMIT + 1];
static size_t _primes_cnt = 0;
static char _is_comp[PRIMELIMIT + 1];

static void _init_primes(void) {
    if (_primes_ready) return;
    memset(_is_comp, 0, sizeof(_is_comp));
    for (uint32_t i = 2; i * i <= PRIMELIMIT; ++i) {
        if (!_is_comp[i]) {
            for (uint32_t j = i * i; j <= PRIMELIMIT; j += i) _is_comp[j] = 1;
        }
    }
    for (uint32_t i = 2; i <= PRIMELIMIT; ++i) {
        if (!_is_comp[i]) _primes[_primes_cnt++] = i;
    }
    _primes_ready = true;
}

/* Factor a 64‑bit integer into primes; result arrays must be large enough (≥16) */
static void _factor_uint64(uint64_t n, uint64_t *prime_out, uint32_t *exp_out, size_t *cnt_out) {
    size_t cnt = 0;
    for (size_t i = 0; i < _primes_cnt && (uint64_t)_primes[i] * (uint64_t)_primes[i] <= n; ++i) {
        uint32_t p = _primes[i];
        if (n % p == 0) {
            uint32_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            prime_out[cnt] = p;
            exp_out[cnt] = e;
            ++cnt;
        }
    }
    if (n > 1) {
        prime_out[cnt] = n;
        exp_out[cnt] = 1;
        ++cnt;
    }
    *cnt_out = cnt;
}

/* Test whether k^n is refactorable. */
static bool _is_refactorable(uint64_t k, uint64_t n) {
    uint64_t k_primes[16];
    uint32_t k_exps[16];
    size_t kcnt = 0;
    _factor_uint64(k, k_primes, k_exps, &kcnt);

    uint64_t d_primes[32];
    uint32_t d_exps[32];
    size_t dcnt = 0;

    for (size_t i = 0; i < kcnt; ++i) {
        uint64_t e = k_exps[i];
        uint64_t term = n * e + 1;     /* ≤ about 200 000 for given limits */
        uint64_t t_primes[16];
        uint32_t t_exps[16];
        size_t tcnt = 0;
        _factor_uint64(term, t_primes, t_exps, &tcnt);
        for (size_t j = 0; j < tcnt; ++j) {
            uint64_t p = t_primes[j];
            uint32_t e_t = t_exps[j];
            bool found = false;
            for (size_t s = 0; s < dcnt; ++s) {
                if (d_primes[s] == p) {
                    d_exps[s] += e_t;
                    found = true;
                    break;
                }
            }
            if (!found) {
                d_primes[dcnt] = p;
                d_exps[dcnt] = e_t;
                ++dcnt;
            }
        }
    }

    /* Validate each prime appearing in d(k^n) */
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t p = d_primes[i];
        uint32_t e_d = d_exps[i];
        bool found_in_k = false;
        uint32_t e_k = 0;
        for (size_t j = 0; j < kcnt; ++j) {
            if (k_primes[j] == p) {
                e_k = k_exps[j];
                found_in_k = true;
                break;
            }
        }
        if (!found_in_k) return false;
        if ((uint64_t)e_d > (uint64_t)e_k * n) return false;
    }
    return true;
}

/* Cache for computed a(n) values */
static seq_t *_cache = NULL;
static idx_t _cache_size = 0;

static void _ensure_cache(idx_t n) {
    if (_cache == NULL) {
        _cache_size = 1; /* placeholder for index 0 */
        _cache = (seq_t *)calloc(_cache_size + 1, sizeof(seq_t));
    } else if (n >= _cache_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(_cache, new_size * sizeof(seq_t));
        if (tmp == NULL) exit(1); /* allocation failed */
        /* Zero new entries */
        for (size_t i = _cache_size; i < new_size; ++i) tmp[i] = 0;
        _cache = tmp;
        _cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    _init_primes();
    _ensure_cache(n);
    if (_cache[n] != 0) return _cache[n];

    uint64_t k = 2;
    while (true) {
        if (_is_refactorable(k, (uint64_t)n)) {
            _cache[n] = (seq_t)k;
            return _cache[n];
        }
        ++k;
    }
}
