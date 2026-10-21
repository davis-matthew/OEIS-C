/*
 * A297279  Numbers whose base-13 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th integer whose base‑13 digits have greater
 *            down‑variation than up‑variation
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool qualifies(seq_t x)
{
    /* Return true if base‑13 down‑variation > up‑variation. */
    seq_t t = x;
    uint8_t digits[128];
    size_t len = 0;

    /* Extract base‑13 digits (least significant first). */
    while (t != (seq_t)0) {
        digits[len++] = (uint8_t)(t % (seq_t)13);
        t = t / (seq_t)13;
    }
    if (len == 0) {        /* x == 0, not a candidate */
        return false;
    }

    seq_t DV = (seq_t)0, UV = (seq_t)0;

    /* Scan from most significant to least significant digit. */
    for (size_t i = 1; i < len; ++i) {
        uint8_t higher = digits[len - i];       /* d(i)   */
        uint8_t lower  = digits[len - i - 1];   /* d(i-1) */
        if (higher > lower) {
            DV += (seq_t)(higher - lower);
        } else if (lower > higher) {
            UV += (seq_t)(lower - higher);
        }
    }
    return DV > UV;
}

seq_t a(idx_t n)
{
    /* Memoized generator of the sequence. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_len  = 0;
    static seq_t  current    = (seq_t)1;

    /* If we already have this term cached, return it. */
    if (n <= (idx_t)cache_len) {
        return cache[n - 1];
    }

    /* Initialize cache if needed. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have the n-th one. */
    while (cache_len < n) {
        if (qualifies(current)) {
            /* Grow cache if necessary. */
            if (cache_len >= (idx_t)cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = current;
        }
        current = current + (seq_t)1;
    }

    return cache[n - 1];
}
