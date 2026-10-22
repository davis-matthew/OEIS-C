/*
 * A369772  a(1) = 1, a(2) = 2; for n >= 3, a(n) = a(GCD(n - 1, a(n - 1))) +
 *          a(n - GCD(n - 1, a(n - 2))).
 * https://oeis.org/A369772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2; for n >= 3, a(n) = a(gcd(n-1, a(n-1))) + a(n
 *            - gcd(n-1, a(n-2))) (OEIS A369772)
 * Range:     u8 n <= 160, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t * _cache   = NULL;           /* 1‑based cache of computed terms   */
static size_t _cache_cap = 0;             /* current capacity of _cache       */
static idx_t  _max_computed = 0;          /* highest n for which we computed */

/* Ensure capacity of the cache for at least n elements (1‑based). */
static void _ensure_cache(idx_t n)
{
    if (_cache_cap <= (size_t)n) {
        size_t new_cap = (_cache_cap == 0) ? 16 : _cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(_cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failure */
        _cache = tmp;
        _cache_cap = new_cap;
    }
}

/* Compute a(i) for all i <= n, storing results in _cache. */
static void _compute_upto(idx_t n)
{
    if (n <= _max_computed) return;       /* nothing to do */
    _ensure_cache(n);

    for (idx_t i = _max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            _cache[1] = 1;
        } else if (i == 2) {
            _cache[2] = 2;
        } else {
            seq_t g1 = gcd((seq_t)(i - 1), _cache[i - 1]);
            idx_t idx1 = (idx_t)g1;                       // g1 <= i-1

            seq_t g2 = gcd((seq_t)(i - 1), _cache[i - 2]);
            idx_t idx2 = i - (idx_t)g2;                  // >= 1

            _cache[i] = _cache[idx1] + _cache[idx2];
        }
    }
    _max_computed = n;
}

seq_t a(idx_t n)
{
    _compute_upto(n);
    return _cache[n];
}
