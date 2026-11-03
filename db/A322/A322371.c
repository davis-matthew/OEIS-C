/*
 * A322371  a(n) is the least practical number that is divisible by prime(n).
 * https://oeis.org/A322371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p * k where p = prime(n) and k is the least practical
 *            number such that sigma(k)+1 ≥ p
 * Range:     u8 n <= 8, u16 n <= 79, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 63 terms through n = 2556 (2026-11-03)
 * Helpers:   build_spf, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_spf.h"
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Helper: sigma of a prime power p^e. */
static seq_t _sigma_prime_power(idx_t p, int e)
{
    seq_t sum = (seq_t)1;
    seq_t term = (seq_t)1;
    for (int i = 0; i < e; ++i) {
        term *= (seq_t)p;
        sum += term;
    }
    return sum;
}

/* Global static data for caching practical numbers and SPF table. */
static idx_t * _spf = NULL;          /* smallest prime factor array */
static size_t   _spf_len = 0;
static seq_t *  _practicals = NULL;  /* list of practical numbers */
static seq_t *  _sigs      = NULL;   /* sigma of each practical number */
static size_t   _pract_len = 0;
static size_t   _pract_cap = 0;
static idx_t    _last_n_processed = 0;
static int      _initialized = 0;

/* Ensure SPF table up to given limit.  Rebuilds completely. */
static void _ensure_spf(idx_t limit)
{
    if (_spf_len >= limit) return;
    idx_t *tmp = NULL;
    build_spf((idx_t)limit, &tmp);
    if (_spf) free(_spf);
    _spf = tmp;
    _spf_len = limit;
}

/* factors n using _spf and checks practicality; returns sigma if outptr not NULL. */
static int _practical_and_sigma(idx_t n, seq_t *out_sigma)
{
    if (n == 1) {
        if (out_sigma) *out_sigma = (seq_t)1;
        return 1;
    }
    /* factorization */
    idx_t nn = n;
    idx_t primes[32];
    int   exps[32];
    int   cnt = 0;
    while (nn > 1) {
        idx_t p = _spf[nn];
        int e = 0;
        while (nn % p == 0) {
            nn /= p;
            ++e;
        }
        primes[cnt] = p;
        exps[cnt]   = e;
        ++cnt;
    }
    if (primes[0] != 2) return 0;     /* must start with 2 */
    seq_t sigma_prev = _sigma_prime_power((idx_t)2, exps[0]);
    for (int i = 1; i < cnt; ++i) {
        if (primes[i] > sigma_prev + (seq_t)1) return 0;
        sigma_prev *= _sigma_prime_power(primes[i], exps[i]);
    }
    if (out_sigma) *out_sigma = sigma_prev;
    return 1;
}

/* Generates practical numbers up to 'limit', appending to global lists. */
static void _generate_practicals(idx_t limit)
{
    for (idx_t n = _last_n_processed + 1; n <= limit; ++n) {
        seq_t sigma;
        if (_practical_and_sigma(n, &sigma)) {
            if (_pract_len == _pract_cap) {
                size_t new_cap = _pract_cap ? _pract_cap * 2 : 128;
                seq_t *new_practs = mem_new(seq_t, new_cap);
                seq_t *new_sigs   = mem_new(seq_t, new_cap);
                if (_pract_cap) {
                    mem_copy(new_practs, _practicals, _pract_len * sizeof(seq_t));
                    mem_copy(new_sigs, _sigs, _pract_len * sizeof(seq_t));
                    mem_free(_practicals);
                    mem_free(_sigs);
                }
                _practicals = new_practs;
                _sigs       = new_sigs;
                _pract_cap  = new_cap;
            }
            _practicals[_pract_len] = (seq_t)n;
            _sigs[_pract_len]       = sigma;
            ++_pract_len;
            _last_n_processed = n;
        }
    }
}

/* Initialization routine called once. */
static void _init(void)
{
    _ensure_spf(200000);           /* initial SPF limit */
    _generate_practicals(200000);  /* generate practical list up to that limit */
    _initialized = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!_initialized) _init();

    seq_t p = (seq_t)get_prime(n);   /* p = prime(n) */

    /* search for smallest practical k with sigma(k)+1 >= p */
    for (size_t i = 0; i < _pract_len; ++i) {
        if (_sigs[i] + (seq_t)1 >= p) {
            return p * _practicals[i];
        }
    }

    /* if not found, extend the list until condition holds */
    idx_t limit = _last_n_processed + 1;
    while (1) {
        limit *= 2;
        _ensure_spf(limit);
        _generate_practicals(limit);
        for (size_t i = 0; i < _pract_len; ++i) {
            if (_sigs[i] + (seq_t)1 >= p) {
                return p * _practicals[i];
            }
        }
    }
}
