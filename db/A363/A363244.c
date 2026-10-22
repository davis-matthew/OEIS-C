/*
 * A363244  Numbers that in primorial-base representation have digits with an
 *          alternating parity.
 * https://oeis.org/A363244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th nonnegative integer whose primorial‑base
 *            representation has digits of alternating parity (e.g. 10110…)
 * Range:     u8 n <= 35, u16 n <= 1574, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm builds the sequence lazily.  It keeps a static cache of
   consecutive terms.  On each call, it extends the cache until it
   contains the requested n‑th term and then returns it.  The validity
   test is performed by converting a number to primorial base and
   checking that adjacent digits have different parity.  This approach
   runs in about O(n · log n) time, which is well below the 2 s limit
   for n ≤ 10 000. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---- static list of first 50 primes ---- */
static const uint64_t _primes[50] = {
    2,   3,   5,   7,  11,  13,  17,  19,  23,  29,
   31,  37,  41,  43,  47,  53,  59,  61,  67,  71,
   73,  79,  83,  89,  97, 101, 103, 107, 109, 113,
  127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
  179, 181, 191, 193, 197, 199, 211, 223, 227, 229
};
static const idx_t _prime_count = 50;

/* ---- local cache of computed terms ---- */
static seq_t * _cache      = NULL;   /* array of terms */
static size_t  _cache_cap  = 0;      /* allocated capacity */
static idx_t   _cache_len  = 0;      /* number of terms stored */
static seq_t   _next_test  = 0;      /* next integer to test */

/* ---- test whether a number satisfies the property ---- */
static bool _valid(seq_t x)
{
    seq_t n = x;
    int prev_parity = -1;                  /* -1 indicates no digit yet */
    for (idx_t i = 0; n > 0; ++i) {
        /* base for digit i is the (i+1)-th prime */
        seq_t base = (seq_t)_primes[i];
        seq_t d    = n % base;   /* current digit */
        n        /= base;        /* move to next position */

        int parity = (int)(d & 1);
        if (prev_parity != -1 && parity == prev_parity)
            return false;          /* parity does not alternate */
        prev_parity = parity;
    }
    return true;  /* 0 has no digits – considered valid */
}

/* ---- resize the cache if needed ---- */
static void _ensure_cache_capacity(void)
{
    if (_cache_len < _cache_cap)
        return;                 /* already enough space */

    size_t new_cap = _cache_cap ? _cache_cap * 2 : 64;
    seq_t *newp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    _cache      = newp;
    _cache_cap  = new_cap;
}

/* ---- main routine ---- */
seq_t a(idx_t n)
{
    /* extend cache until we have at least n terms (1‑based index) */
    while (_cache_len < n) {
        _ensure_cache_capacity();

        if (_valid(_next_test))
            _cache[_cache_len++] = _next_test;

        ++_next_test;
    }
    return _cache[n - 1];
}
