/*
 * A242831  Triangular numbers T such that sum of the factorials of digits of
 *          T is prime.
 * https://oeis.org/A242831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth triangular number T such that the sum of factorials
 *            of the decimal digits of T is prime. The function searches
 *            triangular numbers in increasing order and caches the valid ones
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 5307, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* pre‑computed factorials of digits 0–9 */
static const seq_t factorials[10] = {
    1,      /* 0! */
    1,      /* 1! */
    2,      /* 2! */
    6,      /* 3! */
    24,     /* 4! */
    120,    /* 5! */
    720,    /* 6! */
    5040,   /* 7! */
    40320,  /* 8! */
    362880  /* 9! */
};

/* cache for discovered terms */
static size_t _cache_size = 0;
static size_t _cache_cap   = 0;
static seq_t* _cache       = NULL;

/* next index k (1‑based) to generate a triangular number from */
static idx_t _next_k = 1;

/* compute triangular number for a given k */
static seq_t _triangular(idx_t k)
{
    return (seq_t)k * ((seq_t)k + 1) / 2;
}

/* sum of factorials of the decimal digits of v */
static seq_t _sum_factorial_digits(seq_t v)
{
    seq_t sum = 0;
    if (v == 0)
        return factorials[0];
    while (v > 0)
    {
        unsigned d = (unsigned)(v % 10);
        sum += factorials[d];
        v /= 10;
    }
    return sum;
}

/* add a new valid term to the cache, expanding it if needed */
static void _add_to_cache(seq_t val)
{
    if (_cache_size == _cache_cap)
    {
        size_t new_cap = (_cache_cap == 0) ? 4 : _cache_cap * 2;
        seq_t *newp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (!newp) abort();          /* out of memory: terminate */
        _cache    = newp;
        _cache_cap = new_cap;
    }
    _cache[_cache_size++] = val;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    /* the sequence is 1‑based; callers never pass 0, but guard anyway */
    if (n == 0)
        return 0;

    /* generate terms until we have the n‑th one */
    while (_cache_size < (size_t)n)
    {
        seq_t t = _triangular(_next_k);
        seq_t s = _sum_factorial_digits(t);
        if (is_prime_small(s))
            _add_to_cache(t);
        ++_next_k;
    }

    return _cache[n - 1];
}
