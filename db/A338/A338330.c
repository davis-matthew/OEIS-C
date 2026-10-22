/*
 * A338330  Numbers that are neither a power of a prime (A000961) nor is their
 *          set of distinct prime indices pairwise coprime.
 * https://oeis.org/A338330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer that is * not a power of a
 *            prime and whose distinct prime indices * (prime(1)=2,
 *            prime(2)=3, …) are not pairwise coprime. * This implementation
 *            generates the terms on demand using * a simple sieve of
 *            Eratosthenes and factorises each integer * until the requested
 *            i...
 * Range:     u8 n <= 42, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

/* Forward declarations of build‑time types */
extern seq_t a(idx_t n);

/* Static data used by the function */
static bool _initialized = false;
static idx_t _sieve_limit = 2000000;          /* 2 000 000 is ample for n≤10 000 */
static uint32_t *_prime_index = NULL;         /* maps prime → 1‑based index, 0 if not prime */
static uint32_t *_primes = NULL;              /* list of primes up to _sieve_limit */
static size_t   _prime_cnt = 0;               /* number of primes in _primes */
static seq_t   *_cache = NULL;                /* memoised terms of the sequence      */
static size_t   _cache_size = 0;              /* number of elements stored in _cache*/
static idx_t    _next_candidate = 2;           /* next integer to test */

/* Utility: simple integer sqrt using multiplication (primes are small) */
static inline uint64_t _isqrt(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;               /* highest power of four <= x */
    while (bit > 0) {
        uint64_t next = r + bit;
        if (x >= next) {
            x -= next;
            r += bit << 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Build sieve up to _sieve_limit, fill _prime_index and _primes */
static void _build_sieve(void)
{
    /* Allocate array for primality flag */
    char *is_prime = (char*)malloc((_sieve_limit + 1) * sizeof(char));
    if (!is_prime) exit(EXIT_FAILURE);
    memset(is_prime, 1, (_sieve_limit + 1) * sizeof(char));
    is_prime[0] = is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    uint32_t i;
    for (i = 2; (uint64_t)i * i <= _sieve_limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= _sieve_limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (i = 2; i <= _sieve_limit; ++i)
        if (is_prime[i]) ++cnt;

    _prime_cnt = cnt;
    _primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    _prime_index = (uint32_t*)calloc((_sieve_limit + 1), sizeof(uint32_t));
    if (!_primes || !_prime_index) exit(EXIT_FAILURE);

    /* Populate arrays */
    size_t idx = 0;
    for (i = 2; i <= _sieve_limit; ++i) {
        if (is_prime[i]) {
            _primes[idx] = i;
            _prime_index[i] = (uint32_t)(idx + 1);  /* 1‑based index */
            ++idx;
        }
    }

    free(is_prime);
}

/* Test whether an integer n satisfies the sequence condition.
 * n must be >=2.  Returns true if n should be included. */
static bool _is_candidate(uint64_t n)
{
    if (n <= 1) return false;

    /* Factorisation: collect distinct prime indices */
    uint32_t idxs[20];
    size_t idx_sz = 0;
    uint64_t tmp = n;
    for (size_t pi = 0; pi < _prime_cnt; ++pi) {
        uint32_t p = _primes[pi];
        uint64_t pp = (uint64_t)p * p;
        if (pp > tmp) break;
        if (tmp % p == 0) {
            ++idx_sz;
            idxs[idx_sz - 1] = _prime_index[p];
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {        /* remaining prime factor */
        /* tmp is a prime ≤ _sieve_limit by construction */
        ++idx_sz;
        idxs[idx_sz - 1] = _prime_index[(uint32_t)tmp];
    }

    /* Exclude if only one distinct prime factor (prime power) */
    if (idx_sz <= 1) return false;

    /* Check pairwise coprimality of the indices */
    for (size_t i = 0; i < idx_sz; ++i) {
        for (size_t j = i + 1; j < idx_sz; ++j) {
            uint32_t a = idxs[i];
            uint32_t b = idxs[j];
            /* compute gcd by Euclid */
            while (b != 0) {
                uint32_t t = a % b;
                a = b;
                b = t;
            }
            if (a > 1) return true;   /* found a pair with gcd>1 */
        }
    }

    return false; /* all pairs coprime -> exclude */
}

/* Main function required by the templates */
seq_t a(idx_t n)
{
    if (n == 0) /* sequence is defined from a(1); safety default */
        return (seq_t)0;

    if (!_initialized) {
        _build_sieve();
        _cache = (seq_t*)malloc(1024 * sizeof(seq_t));
        if (!_cache) exit(EXIT_FAILURE);
        _cache_size = 0;
        _initialized = true;
    }

    /* Extend cache until we have n terms */
    while (_cache_size < (size_t)n) {
        /* Test current candidate */
        if (_is_candidate((uint64_t)_next_candidate)) {
            /* grow cache if needed */
            if (_cache_size == 1024) {
                seq_t *tmp = (seq_t*)realloc(_cache, (_cache_size + 1024) *
                                              sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                _cache = tmp;
            }
            _cache[_cache_size++] = (seq_t)_next_candidate;
        }
        ++_next_candidate;
    }

    return _cache[n - 1];
}
