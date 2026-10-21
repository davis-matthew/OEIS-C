/*
 * A243068  Fixed points of A242420.
 * https://oeis.org/A243068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 2185, u32 n <= 2185, u64 n <= 2185,
 *            u128 n <= 2185, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A243068 (Fixed points of A242420)
 * The number n is in the sequence iff, after factorising n,
 * we keep all occurrences of prime factors except extra copies of the
 * largest prime factor.  Let the indices of the surviving prime factors
 * (indices of primes in the standard prime list, 1‑based) be
 * i1, i2, …, ik (in non‑decreasing order).  Define the delta sequence
 * d1 = i1‑0,  d2 = i2‑i1, …, dk = ik‑i(k‑1).  n is in the sequence
 * exactly when the delta sequence is a palindrome.  This code generates
 * the terms in ascending order and returns the nth term (a(1)=1). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* forward declaration of seq_t and idx_t (provided by the build) */
typedef seq_t seq_t;
typedef idx_t idx_t;

/* ---------- Global state ---------- */
static bool _sieve_inited = false;
static uint32_t * _primes = NULL;          /* primes up to PRIME_LIMIT */
static size_t    _prime_count = 0;
static uint32_t * _prime_index = NULL;     /* map prime -> 1‑based index */
static size_t    _prime_cap   = 0;

static seq_t * _seq_cache    = NULL;       /* cache of sequence terms */
static size_t  _seq_size     = 0;          /* current number of cached terms */
static size_t  _seq_cap      = 0;
static idx_t   _curr_checked = 1;          /* last integer examined */

/* ---------- Sieve of Eratosthenes (pre‑computes primes up to LIMIT) ---------- */
#define PRIME_LIMIT 2000000  /* should be generous enough for n up to ~10000 */

static void _init_sieve(void)
{
    if (_sieve_inited) return;
    _sieve_inited = true;

    /* allocate storage */
    _prime_cap = 200000;   /* initial capacity (will grow if needed) */
    _primes = (uint32_t *)malloc(_prime_cap * sizeof(uint32_t));
    _prime_index = (uint32_t *)calloc(PRIME_LIMIT + 1, sizeof(uint32_t));
    if (!_primes || !_prime_index) exit(1);

    unsigned char *is_comp = (unsigned char *)calloc(PRIME_LIMIT + 1, sizeof(unsigned char));
    if (!is_comp) exit(1);

    /* sieve */
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (_prime_count == _prime_cap) {
                _prime_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(_primes, _prime_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                _primes = tmp;
            }
            _primes[_prime_count] = (uint32_t)i;
            _prime_index[i] = (uint32_t)(_prime_count + 1); /* 1‑based index */
            ++_prime_count;
            if (i * i <= PRIME_LIMIT) {
                for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
}

/* ---------- Helper: check whether m belongs to the sequence ---------- */
static bool _is_in_sequence(idx_t m)
{
    if (m == 1) return true;   /* 1 is trivially present */

    idx_t t = m;
    /* store distinct primes and their exponents */
    uint32_t prime_factors[32];
    int exp_factors[32];
    size_t fac_cnt = 0;
    size_t i = 0;
    for (i = 0; i < _prime_count && (idx_t)_primes[i] * (idx_t)_primes[i] <= t; ++i) {
        uint32_t p = _primes[i];
        if (t % p == 0) {
            int exp = 0;
            while (t % p == 0) {
                t /= p;
                ++exp;
            }
            prime_factors[fac_cnt] = p;
            exp_factors[fac_cnt] = exp;
            ++fac_cnt;
        }
    }
    if (t > 1) {   /* t itself is prime */
        prime_factors[fac_cnt] = (uint32_t)t;
        exp_factors[fac_cnt] = 1;
        ++fac_cnt;
    }

    /* build list of prime indices considering the rule for the largest prime */
    idx_t idx_arr[64];
    size_t len = 0;
    for (size_t j = 0; j < fac_cnt; ++j) {
        uint32_t p = prime_factors[j];
        int exp  = exp_factors[j];
        if (j == fac_cnt - 1) exp = 1;  /* largest prime factor counted once */
        uint32_t idx = _prime_index[p];
        for (int k = 0; k < exp; ++k) {
            idx_arr[len++] = idx;
        }
    }

    /* compute delta sequence and test palindrome */
    int delta[64];
    idx_t prev = 0;
    for (size_t j = 0; j < len; ++j) {
        delta[j] = (int)(idx_arr[j] - prev);
        prev = idx_arr[j];
    }
    for (size_t j = 0; j < len / 2; ++j) {
        if (delta[j] != delta[len - 1 - j])
            return false;
    }
    return true;
}

/* ---------- Cache expansion ---------- */
static void _ensure_cache(idx_t n)
{
    if (n < _seq_size) return;                /* already cached */

    /* expand sequence cache as needed */
    while (_seq_size <= n) {
        ++_curr_checked;
        if (_is_in_sequence(_curr_checked)) {
            /* grow the cache array if needed */
            if (_seq_size == _seq_cap) {
                size_t new_cap = (_seq_cap == 0) ? 2 : (_seq_cap * 2);
                seq_t *tmp = (seq_t *)realloc(_seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                _seq_cache = tmp;
                _seq_cap = new_cap;
            }
            _seq_cache[_seq_size++] = (seq_t)_curr_checked;
        }
    }
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    /* initialise once */
    if (!_sieve_inited) _init_sieve();
    if (_seq_cache == NULL) {
        /* first cache entry: dummy 0 at index 0, 1 at index 1 */
        _seq_cap = 2;
        _seq_cache = (seq_t *)malloc(_seq_cap * sizeof(seq_t));
        if (!_seq_cache) exit(1);
        _seq_cache[0] = 0;
        _seq_cache[1] = (seq_t)1;
        _seq_size    = 2;
        _curr_checked = 1;
    }

    _ensure_cache(n);
    return _seq_cache[n];
}
