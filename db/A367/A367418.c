/*
 * A367418  The exponentially odd numbers (A268335) divided by their
 *          squarefree kernels (A007947).
 * https://oeis.org/A367418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A268335(n) / A367417(n) (The n-th exponentially odd
 *            number divided by its square‑free kernel. The result is always a
 *            perfect square.)
 * Range:     u8 n <= 365, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_exponentially_odd_and_rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/_is_exponentially_odd_and_rad.h"

/* Cache of already computed a(n) values. */
static seq_t *_a_cache = NULL;
static size_t _a_cache_size = 0;
static idx_t _next_candidate = 1;  /* integer to test */

/* Ensure enough space in the cache. */
static void _ensure_cache_size(size_t need)
{
    if (_a_cache_size >= need) return;
    size_t new_cap = (_a_cache_size == 0 ? 32 : _a_cache_size * 2);
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(_a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    _a_cache = tmp;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined offset; safe return */

    /* generate terms until the cache contains at least n values */
    while (_a_cache_size < (size_t)n) {
        idx_t x = _next_candidate++;
        seq_t rad;
        if (_is_exponentially_odd_and_rad(x, &rad)) {
            seq_t val = (seq_t)x / rad;  /* divide using seq_t arithmetic */
            _ensure_cache_size(_a_cache_size + 1);
            _a_cache[_a_cache_size++] = val;
        }
    }
    return _a_cache[n - 1];
}
