/*
 * A322007  a(n) = number of primes of the form p = 2n - q, where q is a prime
 *          or semiprime.
 * https://oeis.org/A322007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p where 2n - p is a prime or a semiprime
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the value for any n >= 0, using a cached sieve */
/* that grows as needed. The algorithm is O(π(2n)) per call and runs well */
/* below the 2‑second limit for n up to at least 10 000. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Cached sieve state */
static idx_t  _sieve_limit  = 0;    /* highest number sieved so far   */
static bool   *_is_prime    = NULL; /* true if index is prime         */
static uint32_t *_spf       = NULL; /* smallest prime factor array     */
static uint32_t *_prime_list = NULL;/* array of primes                 */
static size_t   _prime_cnt  = 0;    /* number of primes in _prime_list */

static void _ensure_sieve(idx_t limit)
{
    if (_sieve_limit >= limit) return;   /* already sufficient */

    /* free previous data */
    free(_is_prime);
    free(_spf);
    free(_prime_list);
    _is_prime = NULL;
    _spf      = NULL;
    _prime_list = NULL;
    _sieve_limit = 0;

    /* allocate new storage */
    size_t sz = (size_t)limit + 1;
    _is_prime    = (bool*)    malloc(sz * sizeof(bool));
    _spf         = (uint32_t*)malloc(sz * sizeof(uint32_t));
    _prime_list  = (uint32_t*)malloc(sz * sizeof(uint32_t));

    if (_is_prime == NULL || _spf == NULL || _prime_list == NULL) {
        free(_is_prime);  free(_spf);  free(_prime_list);
        _is_prime    = NULL;
        _spf         = NULL;
        _prime_list  = NULL;
        return;                     /* allocation failed */
    }

    /* initialise arrays */
    for (size_t i = 0; i < sz; ++i) {
        _is_prime[i] = true;
        _spf[i]      = (uint32_t)i;
    }
    if (sz > 0) _is_prime[0] = false;
    if (sz > 1) _is_prime[1] = false;

    /* Eratosthenes sieve, also filling _spf */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (_is_prime[i]) {
            uint32_t sp = (uint32_t)i;
            for (size_t j = i * i; j <= (size_t)limit; j += i) {
                if (_is_prime[j]) _is_prime[j] = false;
                if (_spf[j] == j) _spf[j] = sp;
            }
        }
    }

    /* build prime list */
    _prime_cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (_is_prime[i]) _prime_list[_prime_cnt++] = (uint32_t)i;
    }
    _sieve_limit = limit;
}

/*--------------------------------------------------------------------*/
/*  sequence function                                                */
/*--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    idx_t limit = n + n;                /* 2n is the maximal p and q   */
    _ensure_sieve(limit);
    if (_is_prime == NULL) return 0;    /* allocation failure          */

    seq_t count = 0;

    for (size_t idx = 0; idx < _prime_cnt; ++idx) {
        seq_t p = (seq_t)_prime_list[idx];
        if (p > limit) break;

        seq_t q = limit - p;
        if (q < 2) continue;             /* q must be at least 2        */

        /* q prime ? */
        if (_is_prime[(size_t)q]) {
            ++count;
            continue;
        }

        /* q composite: check if semiprime (product of two primes) */
        uint32_t f1 = _spf[(size_t)q];
        if (f1 != q) {                   /* q is not prime */
            uint32_t f2 = (uint32_t)(q / f1);
            if (f1 <= limit && f2 <= limit &&
                _is_prime[(size_t)f1] && _is_prime[(size_t)f2]) {
                ++count;                 /* semiprime found          */
            }
        }
    }

    return count;
}
