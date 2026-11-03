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
 * Validated: standard, pass, 81 terms through n = 9857 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether x is a member of the sequence. */
static bool _is_seq_term(seq_t x)
{
    /* Count power of two factor. */
    seq_t tmp = x;
    idx_t a = 0;
    while ((tmp & (seq_t)1) == 0) {
        tmp >>= 1;
        a++;
    }

    /* m = a + 1; odd numbers correspond to m = 1. */
    idx_t m = a + 1;
    if (m == 1)                 /* all odd numbers belong. */
        return (tmp & (seq_t)1) != 0;

    /* Find integer d such that d^m == tmp. */
    seq_t d = 1;
    while (1) {
        seq_t prod = 1;
        for (idx_t i = 0; i < m; ++i) {
            prod *= d;
            if (prod > tmp) break;       /* early exit if already exceeded */
        }
        if (prod == tmp) return true;
        if (prod > tmp) break;
        d++;
    }
    return false;
}

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
    _cache = mem_resize(seq_t, _cache, newcap);
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
