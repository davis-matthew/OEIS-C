/*
 * A330406  a(n) is the smallest prime q such that q^((p-1)/2) == -1 (mod p),
 *          where p = A002144(n) is the n-th prime congruent to 1 mod 4.
 * https://oeis.org/A330406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   build_primes, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A330406: a(n) = smallest prime q such that q^((p-1)/2) == -1 (mod p),
   where p is the n-th prime congruent to 1 (mod 4).  The value is
   computed by a naive search over the primes below p.  An initial
   list of primes up to a fixed limit is built once; the cache of
   a(n) values is extended on demand. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes.h"
#include "../utils/mod_pow.h"

static bool _a330406_initialized = false;
static const idx_t _sieve_limit = 3000000;       /* primes up to this limit */

static idx_t * _prime_list      = NULL;   /* all primes up to _sieve_limit          */
static size_t   _prime_count    = 0;       /* number of primes in _prime_list         */
static idx_t * _primes1mod4     = NULL;   /* primes ≡ 1 (mod 4) within the limit     */
static size_t   _primes1_count  = 0;       /* number of primes in _primes1mod4        */

static seq_t * _a_cache = NULL;   /* cache of computed a(n) values   */
static size_t  _cache_size = 0;   /* number of a(n) values cached    */

static void _ensure_cache(idx_t n)
{
    if (!_a330406_initialized) {
        /* build a list of all primes up to the limit */
        build_primes(_sieve_limit, &_prime_list, &_prime_count);

        /* collect the primes that are ≡ 1 (mod 4) */
        for (size_t i = 0; i < _prime_count; ++i)
            if ((_prime_list[i] & 3) == 1)
                ++_primes1_count;

        _primes1mod4 = (idx_t *)malloc(_primes1_count * sizeof(idx_t));
        size_t idx = 0;
        for (size_t i = 0; i < _prime_count; ++i)
            if ((_prime_list[i] & 3) == 1)
                _primes1mod4[idx++] = _prime_list[i];

        _a_cache = NULL;
        _cache_size = 0;
        _a330406_initialized = true;
    }

    if (n <= _cache_size)
        return;  /* nothing to do */

    /* extend the cache up to n */
    size_t new_size = _cache_size;
    _a_cache = (seq_t *)realloc(_a_cache, n * sizeof(seq_t));

    for (size_t k = _cache_size; k < (size_t)n; ++k) {
        /* k = 0 corresponds to a(1) */
        idx_t p = _primes1mod4[k];
        uint64_t exp = ((uint64_t)p - 1) >> 1;   /* (p-1)/2 */
        seq_t target = (seq_t)p - 1;             /* -1 mod p */

        seq_t q_ans = 0;
        for (size_t j = 0; j < _prime_count && _prime_list[j] < p; ++j) {
            seq_t r = mod_pow((seq_t)_prime_list[j], exp, (seq_t)p);
            if (r == target) {
                q_ans = _prime_list[j];
                break;
            }
        }
        _a_cache[k] = q_ans;
    }
    _cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for n < 1 */
        return 0;
    _ensure_cache(n);
    return _a_cache[n - 1];
}
