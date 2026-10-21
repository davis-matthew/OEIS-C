/*
 * A293205  Numbers n > 0 such that 2*n = (4*k-2)^m where k, m > 0.
 * https://oeis.org/A293205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest integer satisfying 2*a(n) = (4*k-2)^m
 *            for some integers k>0, m>0. Equivalently, a(n) = 2^(m-1) * o^m
 *            where o is odd
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_seq_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_is_seq_term.h"

/* Cached sequence values. */
static seq_t  *_cache   = NULL;
static idx_t   _len     = 0;   /* number of terms cached */
static idx_t   _cap     = 0;   /* allocated capacity of cache */
static idx_t   _tested  = 1;   /* next integer to test for membership */

/* Ensure cache has at least 'need' capacity. */
static void _ensure_capacity(idx_t need)
{
    if (_cap >= need) return;
    idx_t newcap = _cap ? _cap << 1 : 64;
    while (newcap < need) newcap <<= 1;
    _cache = (seq_t *)realloc(_cache, newcap * sizeof(seq_t));
    if (!_cache) abort();
    _cap   = newcap;
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Grow cache until it contains at least n terms. */
    while (_len < n) {
        if (_is_seq_term((seq_t)_tested)) {
            _ensure_capacity(_len + 1);
            _cache[_len++] = (seq_t)_tested;
        }
        _tested++;
    }
    return _cache[n - 1];
}
