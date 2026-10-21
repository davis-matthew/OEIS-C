/*
 * A288380  a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - 3*a(n-4) + a(n-5) for n >=
 *          1, where a(0) = 2, a(1) = 4, a(2) = 7. a(3) = 11, a(4) = 20.
 * https://oeis.org/A288380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - 3*a(n-4) + a(n-5), a(0)
 *            = 2, a(1) = 4, a(2) = 7, a(3) = 11, a(4) = 20
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 34, u64 n <= 69, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t _init[5] = { (seq_t)2, (seq_t)4, (seq_t)7, (seq_t)11, (seq_t)20 };
static seq_t * _cache = NULL;
static idx_t _cached_len = 0;
static idx_t _cached_cap = 0;

static void _ensure_cache(idx_t n)
{
    if (_cache == NULL) {
        _cached_cap = (n > 4 ? n + 1 : 5);
        _cache = (seq_t *)malloc(_cached_cap * sizeof(seq_t));
        if (_cache == NULL) exit(1);
        for (idx_t i = 0; i < 5; ++i) _cache[i] = _init[i];
        _cached_len = 5;
    } else if (_cached_cap <= n) {
        idx_t new_cap = (_cached_cap > 0 ? _cached_cap * 2 : 16);
        while (new_cap <= n) new_cap *= 2;
        _cache = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (_cache == NULL) exit(1);
        _cached_cap = new_cap;
    }
    while (_cached_len <= n) {
        seq_t a = _cache[_cached_len - 1];
        seq_t b = _cache[_cached_len - 2];
        seq_t c = _cache[_cached_len - 3];
        seq_t d = _cache[_cached_len - 4];
        seq_t e = _cache[_cached_len - 5];
        seq_t next = (seq_t)3 * a - (seq_t)3 * b + (seq_t)3 * c - (seq_t)3 * d + e;
        _cache[_cached_len] = next;
        ++_cached_len;
    }
}

seq_t a(idx_t n)
{
    if (n < 5) return _init[n];
    _ensure_cache(n);
    return _cache[n];
}
