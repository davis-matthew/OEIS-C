/*
 * A376740  Numbers that have at least one two-digit prime factor.
 * https://oeis.org/A376740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376740 – Numbers that have at least one two‑digit prime factor. */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */
#include <stdint.h>   /* for uint64_t */
#include <stdlib.h>   /* for malloc, realloc, abort */

static const uint64_t _two_digit_primes[] = {
    11,13,17,19,23,29,31,37,41,43,47,
    53,59,61,67,71,73,79,83,89,97
};
#define _TWO_DIGIT_PRIME_COUNT (sizeof _two_digit_primes/sizeof _two_digit_primes[0])

/* Cache of already‑found sequence terms. */
static seq_t  *_cache      = NULL;   /* table of terms */
static idx_t   _cache_cap  = 0;      /* allocated capacity (in terms) */
static idx_t   _cache_len  = 0;      /* number of stored terms */
static idx_t   _examined   = 0;      /* greatest integer that has been examined */

/* Return true iff v has at least one two‑digit prime divisor. */
static inline bool _has_two_digit_prime_factor(seq_t v)
{
    for (size_t i = 0; i < _TWO_DIGIT_PRIME_COUNT; ++i) {
        seq_t p = (seq_t)_two_digit_primes[i];
        if (v % p == 0) return true;
    }
    return false;
}

/* Return the n‑th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset is 1 – out‑of‑range requests return 0 */

    /* Expand cache until it contains at least n terms. */
    while (_cache_len < n) {
        ++_examined;                 /* next integer to test */
        seq_t v = (seq_t)_examined;  /* value to check */

        if (_has_two_digit_prime_factor(v)) {
            /* Grow cache if necessary. */
            if (_cache_len == _cache_cap) {
                idx_t new_cap = _cache_cap ? _cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(_cache, sizeof(seq_t) * new_cap);
                if (!tmp) abort();       /* out of memory – terminate safely */
                _cache     = tmp;
                _cache_cap = new_cap;
            }
            _cache[_cache_len++] = v;  /* store term */
        }
    }

    return _cache[n - 1];
}
