/*
 * A377029  a(1) = 0; thereafter in the binary expansion of a(n-1), expand
 *          bits: 1->01 and 0->10.
 * https://oeis.org/A377029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 0; for n > 1, a(n) is obtained from the binary *
 *            representation of a(n-1) by replacing each 1 with "01" and each
 *            0 with "10", * then discarding any leading zeros
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/bitlen_seq_t.h"


/* Helper that expands a(n-1) to produce a(n). */
static seq_t next_value(seq_t x)
{
    /* Special case for x == 0 (a(1)). */
    if (x == (seq_t)0)
        return (seq_t)2;           /* "10" = 2 */

    /* Find the most significant bit index. */
    idx_t msb = bitlen_seq_t(x) - 1;   /* bitlen_seq_t(x) >= 1 for x>0 */

    seq_t y = 0;
    int bit;

    for (idx_t pos = msb; ; --pos) {
        bit = (int)((x >> pos) & (seq_t)1);

        if (pos == msb) {               /* first bit: drop leading zero of 01 if needed */
            y = (bit == 1) ? (seq_t)1 : (seq_t)2;   /* 01 -> 1, 10 -> 2 */
        } else {
            if (bit == 1)
                y = (y << 2) | (seq_t)1;   /* 01 */
            else
                y = (y << 2) | (seq_t)2;   /* 10 */
        }

        if (pos == 0)
            break;
    }
    return y;
}

/* Cache for memoised values. */
static seq_t *a_cache    = NULL; /* 0-based: a_cache[0] == a(1) */
static size_t a_count    = 0;    /* number of cached terms */
static size_t a_capacity = 0;    /* allocated capacity */

/* Ensure the cache has capacity for at least 'needed' entries (1‑based). */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= a_capacity)
        return;

    size_t new_cap = (a_capacity > 0) ? a_capacity : 1;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* out of memory */

    a_cache = tmp;
    /* Zero‑initialise any new elements (not strictly needed). */
    for (size_t i = a_capacity; i < new_cap; ++i)
        a_cache[i] = (seq_t)0;

    a_capacity = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;           /* undefined for n < 1; harmless */

    size_t ni = (size_t)n;          /* safe conversion for cache sizing */

    ensure_cache_capacity(ni);

    /* First term a(1) = 0, initialise if necessary. */
    if (a_count == 0) {
        a_cache[0] = (seq_t)0;
        a_count = 1;
    }

    if (ni <= a_count)
        return a_cache[ni - 1];

    /* Compute missing terms incrementally. */
    for (size_t i = a_count; i < ni; ++i)
        a_cache[i] = next_value(a_cache[i - 1]);

    a_count = ni;
    return a_cache[ni - 1];
}
