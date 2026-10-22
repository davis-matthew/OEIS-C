/*
 * A374639  Numbers k such that the leaders of maximal anti-runs in the k-th
 *          composition in standard order (A066099) are not distinct.
 * https://oeis.org/A374639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   has_duplicate_leaders
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374639: computed by enumerating compositions of k
 * (standard order A066099) and testing whether the leaders of
 * maximal anti‑runs contain a duplicate.  The function returns
 * the n‑th such k. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_duplicate_leaders.h"

/* static cache for computed terms */
static idx_t      _a_cache_last_k = 0;      /* last k that was considered */
static seq_t    *_a_cache_seq   = NULL;     /* cache array of values */
static size_t     _a_cache_len   = 0;        /* number of cached terms */
static size_t     _a_cache_cap   = 0;        /* current capacity of cache */

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;                           /* undefined for offset, but harmless */

    /* grow cache until it contains the requested term */
    if (_a_cache_len < n) {
        /* increase capacity if necessary */
        size_t need = n;
        if (_a_cache_cap < need) {
            size_t new_cap = (_a_cache_cap > 0) ? _a_cache_cap : 64;
            while (new_cap < need) new_cap <<= 1;
            _a_cache_seq = (seq_t*)realloc(_a_cache_seq, new_cap * sizeof(seq_t));
            _a_cache_cap = new_cap;
        }

        idx_t k = _a_cache_last_k + 1;
        while (_a_cache_len < n) {
            if (has_duplicate_leaders(k))
                _a_cache_seq[_a_cache_len++] = (seq_t)k;
            ++k;
        }
        _a_cache_last_k = k - 1;
    }

    return _a_cache_seq[n - 1];
}
