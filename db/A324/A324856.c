/*
 * A324856  Numbers divisible by exactly one of their prime indices.
 * https://oeis.org/A324856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers n such that exactly one of its prime indices divides n
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The term a(n) (1‑based) is the n‑th such number. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Static state for the sieve, prime indexing and result caching        */
/* --------------------------------------------------------------------- */
static bool          _initialized = false;
static idx_t         _limit = 0;
static uint32_t     *_spf = NULL;                 /* smallest prime factor */
static uint32_t     *_prime_index_of = NULL;      /* index of each prime   */
static uint32_t     *_primes = NULL;              /* list of primes        */
static size_t        _prime_cnt = 0;

static seq_t        *_cache = NULL;              /* collected terms       */
static size_t        _cache_len = 0;
static size_t        _cache_cap = 0;

/* --------------------------------------------------------------------- */
/* Helper: rebuild the sieve and cache up to a new maximum value        */
/* --------------------------------------------------------------------- */
static void _rebuild(idx_t new_limit)
{
    /* free old data */
    free(_spf);
    free(_prime_index_of);
    free(_primes);
    free(_cache);

    _limit = new_limit;
    _cache_len = 0;
    _cache_cap = 0;
    _cache = NULL;

    /* allocate arrays */
    _spf = (uint32_t *)calloc((size_t)_limit + 1, sizeof(uint32_t));
    if (!_spf) abort();

    _prime_index_of = (uint32_t *)calloc((size_t)_limit + 1, sizeof(uint32_t));
    if (!_prime_index_of) abort();

    /* initial sieve values: spf[i] = i */
    for (size_t i = 0; i <= (size_t)_limit; ++i)
        _spf[i] = (uint32_t)i;

    /* Sieve of Eratosthenes to fill spf */
    for (size_t i = 2; i * i <= (size_t)_limit; ++i) {
        if (_spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j <= (size_t)_limit; j += i)
                if (_spf[j] == (uint32_t)j)
                    _spf[j] = (uint32_t)i;
        }
    }

    /* collect primes and build prime_index_of */
    _primes = (uint32_t *)malloc(((size_t)_limit / 2 + 100) * sizeof(uint32_t));
    if (!_primes) abort();

    _prime_cnt = 0;
    for (size_t i = 2; i <= (size_t)_limit; ++i) {
        if (_spf[i] == (uint32_t)i) {
            _prime_index_of[i] = (uint32_t)(_prime_cnt + 1); /* 1‑based index */
            _primes[_prime_cnt++] = (uint32_t)i;
        }
    }

    /* shrink primes array to actual size */
    _primes = (uint32_t *)realloc(_primes, _prime_cnt * sizeof(uint32_t));
    if (!_primes) abort();

    /* preparing cache capacity */
    _cache_cap = 1024;
    _cache = (seq_t *)malloc(_cache_cap * sizeof(seq_t));
    if (!_cache) abort();

    /* ----------------------------------------------------------------- */
    /* test each integer up to new_limit for the property                */
    /* ----------------------------------------------------------------- */
    seq_t cand_seq;
    for (idx_t cand = 2; cand <= _limit; ++cand) {
        /* factor cand using spf */
        uint64_t x = (uint64_t)cand;
        int divisors = 0; /* count of (prime index copies) that divide cand */

        while (x > 1 && divisors <= 1) {
            uint32_t p = _spf[(size_t)x];
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                exp++;
            }

            seq_t idx = (seq_t)_prime_index_of[p];
            if (idx == 0) /* should not happen for primes <= limit */
                continue;

            cand_seq = (seq_t)cand;
            for (int i = 0; i < exp && divisors <= 1; ++i)
                if ((cand_seq % idx) == 0)
                    divisors++;
        }

        if (divisors == 1) {
            if (_cache_len == _cache_cap) {
                _cache_cap <<= 1;
                _cache = (seq_t *)realloc(_cache, _cache_cap * sizeof(seq_t));
                if (!_cache) abort();
            }
            _cache[_cache_len++] = (seq_t)cand;
        }
    }

    _initialized = true;
}

/* --------------------------------------------------------------------- */
/* Ensure we have at least needed terms cached                           */
/* --------------------------------------------------------------------- */
static void _ensure_terms(idx_t needed)
{
    if (_initialized && _cache_len >= (size_t)needed)
        return;

    /* start with a reasonable limit */
    idx_t lim = (_limit == 0) ? 2000000U : _limit;
    do {
        _rebuild(lim);
        if (_cache_len >= (size_t)needed)
            return;
        lim <<= 1; /* double the limit */
    } while (1);
}

/* --------------------------------------------------------------------- */
/* Public function required by the build                                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but safe fallback */

    _ensure_terms(n);
    return _cache[(size_t)n - 1];
}
