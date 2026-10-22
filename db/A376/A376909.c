/*
 * A376909  Numbers k such that none of T(k-1), T(k), and T(k+1) is cubefree,
 *          with T(n)=A000217(n).
 * https://oeis.org/A376909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   _tri_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that T(k‑1), T(k), T(k+1) are all not cubefree,
 * where T(n)=n*(n+1)/2.  Returns the nth element of that sequence
 * (1‑based indexing). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_tri_u64.h"

/* Utilities -----------------------------------------------------------*/
static bool _primes_initialized = false;
static uint32_t * _primes = NULL;
static size_t _primes_count = 0;

/* Generate all primes up to a fixed bound (1 000 000) once. */
static void _init_primes(void)
{
    if (_primes_initialized) return;
    const size_t LIMIT = 1000000;
    bool *sieve = (bool*)malloc(LIMIT + 1);
    for (size_t i = 0; i <= LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = false;
    _primes_count = 0;
    _primes = (uint32_t*)malloc((LIMIT / 10) * sizeof(uint32_t));
    for (size_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) _primes[_primes_count++] = (uint32_t)i;
    free(sieve);
    _primes_initialized = true;
}

/* Check whether a positive 64‑bit integer has a cube factor. */
/* If n==0 we treat it as having a cube factor. */
static bool _has_cube_factor(uint64_t n)
{
    if (n == 0) return true;
    for (size_t i = 0; i < _primes_count; ++i) {
        uint32_t p = _primes[i];
        uint64_t p3 = (uint64_t)p * p * p;
        if (p3 > n) break;
        if (n % p == 0) {
            int exp = 0;
            do {
                n /= p;
                ++exp;
                if (exp >= 3) return true;
            } while (n % p == 0);
        }
    }
    /* Remaining n is 1 or a prime with exponent 1 (cannot be a cube). */
    return false;
}

/* Return true iff n is cubefree. */
static bool _is_cubefree(uint64_t n)
{
    return !_has_cube_factor(n);
}

/* Main function ---------------------------------------------*/
static seq_t * _cached_k = NULL;
static size_t _cached_len = 0;
static size_t _cached_cap = 0;
static idx_t   _scan_k = 1;   /* next k to test */

/* Append a new term to the cache. */
static void _append_k(idx_t k)
{
    if (_cached_len == _cached_cap) {
        size_t new_cap = (_cached_cap == 0) ? 64 : _cached_cap * 2;
        seq_t *tmp = (seq_t*)realloc(_cached_k, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        _cached_k = tmp;
        _cached_cap = new_cap;
    }
    _cached_k[_cached_len++] = (seq_t)k;
}

/* Public API ------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but return 0 safely */

    _init_primes();

    /* Generate enough terms to satisfy the requested index. */
    while (_cached_len < n) {
        uint64_t t1 = _tri_u64(_scan_k - 1);
        uint64_t t2 = _tri_u64(_scan_k);
        uint64_t t3 = _tri_u64(_scan_k + 1);

        if (!_is_cubefree(t1) && !_is_cubefree(t2) && !_is_cubefree(t3))
            _append_k(_scan_k);

        ++_scan_k;
    }
    return _cached_k[n - 1];
}
