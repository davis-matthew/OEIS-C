/*
 * A266347  Numbers that cannot be represented as the product of two numbers
 *          with an equal number of significant digits (bits) in their binary
 *          representations.
 * https://oeis.org/A266347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A266347 Numbers that cannot be represented as the product of two
 *            numbers with an equal number of significant bits in their binary
 *            representations. The nth term a(n) is the nth smallest element
 *            of this set, with a(1)=2
 * Range:     u8 n <= 206, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _bitlen_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/_bitlen_u64.h"

/* Integer square root of a 64‑bit unsigned integer (floor). */
static uint64_t _isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = ((uint64_t)1 << 32) - 1;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Does the integer m satisfy the property that no divisor pair (d, m/d)
   has equal bit lengths in binary? */
static bool _qualifies(uint64_t m)
{
    uint64_t root = _isqrt_u64(m);
    for (uint64_t d = 1; d <= root; ++d) {
        if (m % d != 0)
            continue;
        uint64_t e = m / d;
        if (_bitlen_u64(d) == _bitlen_u64(e))
            return false;
    }
    return true;
}

/* Static cache for already found terms of the sequence. */
static seq_t  *seq_cache  = NULL;
static size_t  cache_len  = 0;
static size_t  cache_cap  = 0;
static uint64_t next_candidate = 2;

static void _ensure_cache_capacity(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset 1; undefined for 0, but safe. */

    /* Generate terms until we have at least n terms cached. */
    while (cache_len < n) {
        seq_t val = (seq_t)next_candidate;
        if (_qualifies(next_candidate)) {
            _ensure_cache_capacity(cache_len + 1);
            seq_cache[cache_len++] = val;
        }
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
