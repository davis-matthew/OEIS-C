/*
 * A366860  a(0) = 0; a(n) = n - a(floor(a(n-1)/3)).
 * https://oeis.org/A366860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = n - a(floor(a(n-1)/3)) for n > 0
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence with memoization. */

#include <stddef.h>
#include "../utils/grow_seq_array.h"

static seq_t *_cache = NULL;
static size_t _cap = 0;

/* Ensure the cache can hold indices up to n (inclusive). */
static void _ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) > _cap) {
        size_t old = _cap;
        grow_seq_array(&_cache, &_cap, (size_t)(n + 1));
        /* initialise new slots to zero */
        for (size_t i = old; i < _cap; ++i)
            _cache[i] = (seq_t)0;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        if (!_cache) {
            _ensure_capacity(0);
            _cache[0] = (seq_t)0;
        }
        return _cache[0];
    }

    _ensure_capacity(n);

    if (_cache[n] != (seq_t)0)     /* already computed (a(n) > 0 for n>0) */
        return _cache[n];

    seq_t prev = a(n - 1);                    /* a(n‑1) */
    idx_t idx = (idx_t)(prev / (seq_t)3);     /* floor(a(n-1)/3) */
    seq_t val = (seq_t)n - a(idx);            /* a(n) */

    _cache[n] = val;
    return val;
}
