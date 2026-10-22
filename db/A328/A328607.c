/*
 * A328607  Numbers whose reversed binary expansion, without the most
 *          significant digit, is a necklace.
 * https://oeis.org/A328607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer whose reversed binary expansion, without the
 *            most significant digit, is a necklace (lexicographically minimal
 *            among its cyclic rotations)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_minimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_minimal.h"

/* Return true iff x satisfies the necklace condition. */
static bool is_necklace(idx_t x)
{
    if (x <= 1) return true;          /* 0 and 1 qualify trivially */

    /* Length of binary representation of x.  x != 0 here. */
    unsigned int len = 64 - __builtin_clzll((uint64_t)x);
    unsigned int m   = len - 1;        /* length of the reversed part */

    /* Extract the lower m bits (suffix after removing the MSB). */
    uint64_t suffix = x & ((1ULL << m) - 1ULL);

    /* bits[i] = i‑th character of the reversed string, bits[0] is leftmost. */
    static unsigned char bits[64];
    for (unsigned int i = 0; i < m; ++i)
        bits[i] = (suffix >> i) & 1ULL;

    return is_minimal(bits, m);
}

/* Compute a(n).  After the first call the sequence values are cached. */
seq_t a(idx_t n)
{
    /* Static cache. */
    static seq_t *cache = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    static idx_t   next_value = 0;    /* next integer to test */

    /* Allocate initial cache. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n+1 values. */
    while (cache_len <= n) {
        idx_t cand = next_value;
        while (!is_necklace(cand)) ++cand;   /* find next qualifying integer */

        /* Store the found value. */
        if (cache_len == cache_cap) {
            /* Grow cache. */
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)cand;
        next_value = cand + 1;
    }

    return cache[n];
}
