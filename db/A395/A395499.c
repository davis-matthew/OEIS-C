/*
 * A395499  Number of derangements of [n] with 3 ascents.
 * https://oeis.org/A395499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A395499 – Number of derangements of [n] with 3 ascents
 *
 * Recurrence (valid for n > 13):
 *   a(n) = 16·a(n-1) - 101·a(n-2) + 298·a(n-3) - 278·a(n-4)
 *          - 656·a(n-5) + 1854·a(n-6) - 676·a(n-7)
 *          - 2581·a(n-8) + 2744·a(n-9) + 935·a(n-10)
 *          - 2422·a(n-11) + 460·a(n-12) + 696·a(n-13)
 *          - 288·a(n-14)
 *
 * Initial values (idx 0 … 13):
 *   0,0,0,0,0,8,104,896,5984,34520,180416,881152,4099168,18402472
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Types supplied by the build system */
#ifndef IDX_T_DEFINED
typedef uint64_t idx_t;
#endif
#ifndef SEQ_T_DEFINED
/* seq_t is defined by the build (uint8_t…uint64_t, unsigned __int128, or big‑integer class) */
#endif

/* Compile‑time constants for the starting values */
static const seq_t _init_vals[14] = {
    (seq_t)0,  (seq_t)0,  (seq_t)0,  (seq_t)0,  (seq_t)0,
    (seq_t)8,   (seq_t)104,   (seq_t)896,   (seq_t)5984,
    (seq_t)34520,   (seq_t)180416,   (seq_t)881152,
    (seq_t)4099168, (seq_t)18402472
};

/* Cache for computed sequence values */
static seq_t * _cache = NULL;
static size_t  _cache_cap = 0;
static int    _init_done = 0;

/* Ensure that the cache can hold indices up to n (inclusive) */
static void _ensure_cache(idx_t n)
{
    if ((size_t)n < _cache_cap)
        return;

    size_t new_cap = _cache_cap ? _cache_cap : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();

    _cache = tmp;
    _cache_cap = new_cap;

    /* Copy the known initial terms once */
    if (!_init_done) {
        for (size_t i = 0; i < 14; ++i)
            _cache[i] = _init_vals[i];
        _init_done = 1;
    }
}

/* Compute a(n) ----------------------------------------------- */
seq_t a(idx_t n)
{
    _ensure_cache(n);

    /* Directly return the pre‑computed values for n < 14 */
    if (n < 14)
        return _cache[(size_t)n];

    /* Recurrence constants */
    const seq_t c16   = (seq_t)16;
    const seq_t c101  = (seq_t)101;
    const seq_t c298  = (seq_t)298;
    const seq_t c278  = (seq_t)278;
    const seq_t c656  = (seq_t)656;
    const seq_t c1854 = (seq_t)1854;
    const seq_t c676  = (seq_t)676;
    const seq_t c2581 = (seq_t)2581;
    const seq_t c2744 = (seq_t)2744;
    const seq_t c935  = (seq_t)935;
    const seq_t c2422 = (seq_t)2422;
    const seq_t c460  = (seq_t)460;
    const seq_t c696  = (seq_t)696;
    const seq_t c288  = (seq_t)288;

    for (idx_t i = 14; i <= n; ++i)
    {
        /* Use a wide intermediate type for built‑in integer seq_t to avoid overflow.
           For arbitrary‑precision seq_t the operations themselves are safe. */
        if (sizeof(seq_t) <= 8) /* built‑in integer types */
        {
            unsigned long long v = 0;
            v += (unsigned long long)_cache[(size_t)(i-1)] * (unsigned long long)c16;
            v -= (unsigned long long)_cache[(size_t)(i-2)] * (unsigned long long)c101;
            v += (unsigned long long)_cache[(size_t)(i-3)] * (unsigned long long)c298;
            v -= (unsigned long long)_cache[(size_t)(i-4)] * (unsigned long long)c278;
            v -= (unsigned long long)_cache[(size_t)(i-5)] * (unsigned long long)c656;
            v += (unsigned long long)_cache[(size_t)(i-6)] * (unsigned long long)c1854;
            v -= (unsigned long long)_cache[(size_t)(i-7)] * (unsigned long long)c676;
            v -= (unsigned long long)_cache[(size_t)(i-8)] * (unsigned long long)c2581;
            v += (unsigned long long)_cache[(size_t)(i-9)] * (unsigned long long)c2744;
            v += (unsigned long long)_cache[(size_t)(i-10)] * (unsigned long long)c935;
            v -= (unsigned long long)_cache[(size_t)(i-11)] * (unsigned long long)c2422;
            v += (unsigned long long)_cache[(size_t)(i-12)] * (unsigned long long)c460;
            v += (unsigned long long)_cache[(size_t)(i-13)] * (unsigned long long)c696;
            v -= (unsigned long long)_cache[(size_t)(i-14)] * (unsigned long long)c288;
            _cache[(size_t)i] = (seq_t)v;
        }
        else   /* arbitrary‑precision seq_t (no overflow of intermediate) */
        {
            seq_t v = 0;
            v += _cache[(size_t)(i-1)] * c16;
            v -= _cache[(size_t)(i-2)] * c101;
            v += _cache[(size_t)(i-3)] * c298;
            v -= _cache[(size_t)(i-4)] * c278;
            v -= _cache[(size_t)(i-5)] * c656;
            v += _cache[(size_t)(i-6)] * c1854;
            v -= _cache[(size_t)(i-7)] * c676;
            v -= _cache[(size_t)(i-8)] * c2581;
            v += _cache[(size_t)(i-9)] * c2744;
            v += _cache[(size_t)(i-10)] * c935;
            v -= _cache[(size_t)(i-11)] * c2422;
            v += _cache[(size_t)(i-12)] * c460;
            v += _cache[(size_t)(i-13)] * c696;
            v -= _cache[(size_t)(i-14)] * c288;
            _cache[(size_t)i] = v;
        }
    }

    return _cache[(size_t)n];
}
