/*
 * A243658  a(0)=0; thereafter a(n) = noz(n+a(n-1)), where noz(n) =
 *          A004719(n).
 * https://oeis.org/A243658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, hear
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 2506, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = noz(n + a(n-1)), a(0)=0
 * where noz(x) removes all zero digits from the decimal representation of x.
 *
 * This implementation memoises computed values in a dynamic array.
 * All arithmetic is done with seq_t; indices are cast to size_t for array access.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Remove zero digits from a non‑negative integer. */
static seq_t noz(seq_t x)
{
    if (x == 0) return 0;

    uint8_t digits[32];
    size_t len = 0;

    while (x != 0 && len < sizeof digits) {
        seq_t d = x % 10;
        if (d != 0) digits[len++] = (uint8_t)d;
        x /= 10;
    }

    if (len == 0) return 0;

    seq_t r = 0;
    for (size_t i = len; i-- > 0;) r = r * 10 + digits[i];
    return r;
}

/* Cache for memoisation. */
static seq_t *cache   = NULL;
static size_t cache_len = 0; /* number of entries stored, >= 1 if cache[0] is set */
static size_t cache_cap = 0; /* allocated capacity */

/* Ensure the cache can hold at least 'needed' entries. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();

    cache = new_ptr;
    cache_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    size_t idx = (size_t)n;

    if (cache_len <= idx) {
        size_t needed = idx + 1;
        grow_cache(needed);

        if (cache_len == 0) cache[0] = 0;

        for (size_t k = cache_len; k <= idx; ++k) {
            if (k == 0) continue;          /* already set */
            seq_t sum = (seq_t)k + cache[k - 1];
            cache[k] = noz(sum);
        }
        cache_len = needed;
    }
    return cache[idx];
}
