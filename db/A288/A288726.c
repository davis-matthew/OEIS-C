/*
 * A288726  a(n) = Sum_{i=floor((n-1)/2)..n-1} i * c(i), where c is the prime
 *          characteristic (A010051).
 * https://oeis.org/A288726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=floor((n-1)/2)}^{n-1} i * c(i), where c(i)=1 if i
 *            is prime, 0 otherwise
 * Range:     u8 n <= 53, u16 n <= 1087, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static idx_t _sieve_limit = 0;
static bool   *_sieve_composite = NULL;
static seq_t  *_sieve_prefix   = NULL;
static size_t  _sieve_cap = 0;

/* Extend the sieve and prefix sum up to 'limit' inclusive. */
static void
_extend_limit(idx_t limit)
{
    if (limit <= _sieve_limit)
        return;

    /* Ensure we have at least capacity for indices [0..limit] */
    if (_sieve_cap < (size_t)(limit + 1)) {
        size_t new_cap = limit + 1;
        bool   *new_composite;
        seq_t  *new_prefix;

        new_composite = (bool *)realloc(_sieve_composite,
                                        new_cap * sizeof(bool));
        if (!new_composite) exit(EXIT_FAILURE);
        _sieve_composite = new_composite;

        new_prefix = (seq_t *)realloc(_sieve_prefix,
                                     new_cap * sizeof(seq_t));
        if (!new_prefix) exit(EXIT_FAILURE);
        _sieve_prefix = new_prefix;

        _sieve_cap = new_cap;
    }

    /* Initialise new entries as non‑composite */
    for (idx_t i = _sieve_limit + 1; i <= limit; ++i)
        _sieve_composite[i] = false;

    /* Incremental sieve: mark composites for primes up to sqrt(limit) */
    for (idx_t p = 2; p <= limit / p; ++p) {
        if (!_sieve_composite[p]) {
            idx_t start = p * p;
            if (start <= _sieve_limit) {
                /* Start from the next multiple beyond current limit */
                start = ( (_sieve_limit + 1 + p - 1) / p ) * p;
            }
            for (idx_t j = start; j <= limit; j += p)
                _sieve_composite[j] = true;
        }
    }

    /* Build prefix sum of primes */
    for (idx_t i = _sieve_limit + 1; i <= limit; ++i) {
        if (i >= 2 && !_sieve_composite[i]) {
            _sieve_prefix[i] = _sieve_prefix[i-1] + (seq_t)i;
        } else {
            _sieve_prefix[i] = _sieve_prefix[i-1];
        }
    }

    _sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n <= 1)                 /* a(0)=a(1)=0 by definition */
        return 0;

    idx_t end   = n - 1;
    idx_t start = (n - 1) / 2;  /* floor((n-1)/2) */

    _extend_limit(end);        /* ensure prefix sums up to 'end' */

    seq_t res = _sieve_prefix[end];
    if (start > 0)
        res -= _sieve_prefix[start - 1];

    return res;
}
