/*
 * A263108  Semiprimes m such that the leftward cyclic permutation of its
 *          decimal digits is a larger semiprime.
 * https://oeis.org/A263108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1384, u32 n <= 8952, u64 n <= 8952,
 *            u128 n <= 8952, big any n
 * Helpers:   count_prime_factors, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A263108: semiprimes m such that the leftward cyclic
 * permutation of its decimal digits is a larger semiprime.
 * The implementation scans integers and stores found terms in a
 * static cache; any call to a(n) returns the nth term regardless
 * of call order. */

#include <stdlib.h>
#include <stddef.h>
/* Utilities supplied by the build environment */
#include "../utils/count_prime_factors.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

static size_t  _a_cap   = 0;      /* capacity of the cache */
static size_t  _a_len   = 0;      /* number of cached terms   */
static seq_t * _a_cache = NULL;   /* cache array              */
static seq_t  _a_last   = 0;      /* smallest number not yet checked */

/* Ensure the cache has room for at least 'need' elements. */
static void _a_ensure_capacity(size_t need)
{
    if (need <= _a_cap)
        return;
    size_t new_cap = _a_cap ? _a_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(_a_cache, new_cap * sizeof(seq_t));
    if (tmp)
    {
        _a_cache = tmp;
        _a_cap   = new_cap;
    }
}

/* Return the n-th term (n >= 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but keep defined for any call */

    /* Return cached value if already available. */
    if (n <= _a_len)
        return _a_cache[n - 1];

    /* Generate terms until we have at least n of them. */
    while (_a_len < n)
    {
        ++_a_last;                 /* test next integer */

        /* Skip numbers that are not semiprime. */
        if (count_prime_factors(_a_last) != (seq_t)2)
            continue;

        /* Compute leftward cyclic permutation. */
        int  d   = digit_len(_a_last);           /* number of decimal digits */
        seq_t p10 = pow10((idx_t)(d - 1));        /* 10^(d-1) */
        seq_t high = _a_last / p10;               /* first digit(s) */
        seq_t low  = _a_last % p10;               /* remaining digits */
        seq_t perm = low * 10 + high;             /* leftward rotation */

        /* The rotated number must be larger and also a semiprime. */
        if (perm <= _a_last)
            continue;
        if (count_prime_factors(perm) != (seq_t)2)
            continue;

        /* Store the found term in the cache. */
        _a_ensure_capacity(_a_len + 1);
        _a_cache[_a_len++] = _a_last;
    }

    return _a_cache[n - 1];
}
