/*
 * A332349  Number of maximal independent sets in the 4 X n king graph.
 * https://oeis.org/A332349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 3*a(n-3) - 4*a(n-4) + 6*a(n-5) -
 *            3*a(n-6) - 6*a(n-7) Initial values: a(0)=1, a(1)=3, a(2)=12,
 *            a(3)=22, a(4)=79, a(5)=194, a(6)=537
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init[7] = {1,3,12,22,79,194,537};

static seq_t * _cache   = NULL;
static size_t _cache_cap = 0;
static size_t _cache_len = 0;

seq_t a(idx_t n) {
    if (n < 7) return _init[n];

    /* Allocate or grow cache if needed */
    if (_cache == NULL) {
        _cache_cap = (size_t)n + 1;
        if (_cache_cap < 8) _cache_cap = 8;
        _cache = (seq_t*)malloc(_cache_cap * sizeof(seq_t));
    } else if ((size_t)n + 1 > _cache_cap) {
        _cache_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(_cache, _cache_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        _cache = tmp;
    }

    /* Fill initial values if not already present */
    if (_cache_len < 7) {
        for (size_t i = _cache_len; i < 7; ++i)
            _cache[i] = _init[i];
        _cache_len = 7;
    }

    /* Compute missing entries up to n */
    for (size_t i = _cache_len; i <= (size_t)n; ++i) {
        seq_t pos = _cache[i-1];
        pos = pos + (seq_t)4 * _cache[i-2];
        pos = pos + (seq_t)3 * _cache[i-3];
        pos = pos + (seq_t)6 * _cache[i-5];

        seq_t neg = (seq_t)4 * _cache[i-4];
        neg = neg + (seq_t)3 * _cache[i-6];
        neg = neg + (seq_t)6 * _cache[i-7];

        _cache[i] = pos - neg;
    }

    _cache_len = (size_t)n + 1;
    return _cache[n];
}
