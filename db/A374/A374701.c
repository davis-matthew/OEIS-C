/*
 * A374701  Numbers k such that the leaders of weakly decreasing runs in the
 *          k-th composition in standard order (A066099) are distinct.
 * https://oeis.org/A374701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k-th integer whose leaders of weakly decreasing runs in
 *            the k-th composition (in standard order A066099) are all
 *            distinct. The code generates the terms by brute‑force testing of
 *            compositions, retaining the indices in a static cache for fast
 *            random access
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Test whether the integer k satisfies the distinct‑leader property. */
static bool is_good(uint64_t k)
{
    /* k = 0 corresponds to the composition (1), which is good. */
    if (k == 0)
        return true;

    /* Up to 64 set bits → at most 64 parts plus one extra for reverse. */
    uint8_t parts[64];
    uint8_t m = 0;          /* number of parts */
    uint8_t prev = 0;       /* previous position of a 1 (0 ⇒ prepended 0) */

    /* Iterate over the bits of k.  i=0 is the least significant bit. */
    for (uint8_t i = 0; i < 64; ++i) {
        if (k & ((uint64_t)1 << i)) {
            uint8_t pos = i + 1;               /* 1‑based position in reversed binary */
            parts[m++] = pos - prev;           /* first difference */
            prev = pos;
        }
    }

    /* Reverse the array to obtain the composition in the required order. */
    for (uint8_t i = 0; i < m / 2; ++i) {
        uint8_t t   = parts[i];
        parts[i]    = parts[m - 1 - i];
        parts[m - 1 - i] = t;
    }

    /* Check that leaders of maximal weakly decreasing runs are distinct. */
    bool seen[128] = {0};           /* leaders are ≤ last position (≤ 64) */
    uint8_t leader = parts[0];
    seen[leader] = true;

    for (uint8_t i = 1; i < m; ++i) {
        if (parts[i] > parts[i - 1]) {        /* new run starts */
            leader = parts[i];
            if (seen[leader])
                return false;                 /* duplicate leader */
            seen[leader] = true;
        }
    }
    return true;
}

/* Memoized sequence generator for A374701. */
seq_t a(idx_t n)
{
    if (n == 0)  /* safety for out‑of‑range requests */
        return 0;

    static seq_t *cache   = NULL;   /* cached good indices */
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_k  = 0;     /* candidate k to test next */

    /* Grow cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        if (is_good(next_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *new_mem = (seq_t *)realloc(
                                    cache, new_cap * sizeof(seq_t));
                if (!new_mem)
                    abort();               /* memory error */
                cache = new_mem;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
